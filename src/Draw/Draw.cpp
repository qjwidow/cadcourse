#include "Draw.hpp"

#include <iostream>
#include <vector>

// Global Data Area
GLFWwindow *window; // (In the accompanying source code, this variable is global for simplicity)
const int screen_x = 1280;
const int screen_y = 720;
float z_angle = 0;
float y_angle = 0;
bool keys[1024] = {false};

void CALLBACK tessBeginCB(GLenum which) {
    glBegin(which);
}

void CALLBACK tessEndCB() {
    glEnd();
}

void CALLBACK tessVertexCB(const GLvoid *data) {
    // cast back to double type
    const GLdouble *ptr = (const GLdouble *) data;
    glColor3dv(ptr + 3);
    glVertex3dv(ptr);
}

void keyboard_cb(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
    if (action == GLFW_PRESS){
        if(key == GLFW_KEY_W){
            Draw::drawingType = Draw::TypeWireframe;
        }else if(key == GLFW_KEY_S){
            Draw::drawingType = Draw::TypeSolid;
        }
    }

}

void movement(float deltaTime) {
    deltaTime *= 200;
    // Camera controls
    if (keys[GLFW_KEY_UP])
        y_angle += 1.0f * deltaTime;
    if (keys[GLFW_KEY_DOWN])
        y_angle -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_LEFT])
        z_angle += 1.0f * deltaTime;
    if (keys[GLFW_KEY_RIGHT])
        z_angle -= 1.0f * deltaTime;

}

void Draw::init() {
    if (!glfwInit()) {
        fprintf(stderr, "Fail to initialize GLFW\n");
        exit(1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(screen_x, screen_y, "Solid Modeling using Euler Operation", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = true; // Needed in core profile

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetKeyCallback(window, keyboard_cb);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    // Draw Backgroud Color
    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void Draw::end() {
    glfwTerminate();
}

void Draw::draw(Solid * &solid) {
    GLfloat lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        GLfloat newTime = glfwGetTime();
        movement(newTime - lastTime);
        lastTime = newTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        glViewport(10, 10, screen_x * 2, screen_y * 2);

        // Perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(45.0f, screen_x / 1.0 / screen_y, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        // glTranslatef(0.0f, 0.0f, -10.0f);
        glTranslatef(-3.0f, -1.5f, -10.0f);

        glRotatef(z_angle, 0.0f, 1.0f, 0.0f);
        glRotatef(y_angle, 1.0f, 0.0f, 1.0f);

        glRotatef(180.0, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);

        // Start drawing
        Solid * p = solid;
        while (p) {
            drawSolid(p);
            p = p->Snext;
        }

        // End drawing
        glfwSwapBuffers(window);
    }
}

void Draw::drawSolid(Solid * &solid) {
    Face * f = solid->Sfaces;
    while(f){
        if(drawingType == TypeSolid){
            drawSolidFace(f);
        }else if(drawingType == TypeWireframe){
            drawWireFrameFace(f);
        }else{
            throw std::runtime_error("Unrecognized Drawing Type.");
        }
        f = f->Fnext;
    }
}

void Draw::drawSolidFace(Face * &face) {
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);

    // ------------ Draw inner loop ------------
    // Write 1's into stencil buffer where the hole will be
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, 1, ~0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for(Loop * l = face->Floops; l != nullptr; l = l->Lnext){
        if(l->inner){
            drawSolidLoop(l);
        }
    }

    // ------------ Draw outer loop ------------
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0, ~0);
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    for(Loop * l = face->Floops; l != nullptr; l = l->Lnext){
        if(!l->inner){
            drawSolidLoop(l);
        }
    }

    glDisable(GL_STENCIL_TEST);
}


void Draw::drawSolidLoop(Loop * &loop) {
    GLuint id = glGenLists(1); // create a display list
    if (!id)
        return; // failed to create a list, return 0

    GLUtesselator *tess = gluNewTess(); // create a tessellator
    if (!tess)
        return; // failed to create tessellation object, return 0

    // register callback functions
    gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK *)())tessBeginCB);
    gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK *)())tessEndCB);
    gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK *)())tessVertexCB);

    glNewList(id, GL_COMPILE);
    glColor3f(1, 1, 1);
    gluTessBeginPolygon(tess, 0); // with NULL data
    gluTessBeginContour(tess);
    HalfEdge * startHe = loop->Lhe;
    HalfEdge * he = loop->Lhe;
    Vertex * startV = he->v;
    std::vector<TessData> quadList;
    do
    {
        glm::vec3 point = he->v->cor;
        glm::vec3 color = loop->Lface->color;
        TessData data(point, color);
        quadList.push_back(data);
        he = he->HEnext;
    } while (nullptr != he && he != startHe);
    for (int i = 0; i < quadList.size(); i++)
    {
        gluTessVertex(tess, quadList[i].data, quadList[i].data);
    }

    gluTessEndContour(tess);
    gluTessEndPolygon(tess);
    glEndList();

    gluDeleteTess(tess); // delete after tessellation

    glCallList(id);
}


void Draw::drawWireFrameFace(Face * &face){
    Loop * loop = face->Floops;
    while(loop != nullptr){
        drawWireFrameLoop(loop);
        loop = loop->Lnext;
    }
}

void Draw::drawWireFrameLoop(Loop * &loop){
    HalfEdge * he = loop->Lhe;
    glBegin(GL_LINE_LOOP);

    do{
        if(he == nullptr) break;
        glColor3f(1, 1, 1);
        const glm::vec3 &p = he->v->cor;
        glVertex3f(p.x, p.y, p.z);

        he = he->HEnext;
    }while(he != loop->Lhe);

    glEnd();
}