#ifndef DRAW_HPP
#define DRAW_HPP

#include "HalfEdgeDS.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef CALLBACK
#define CALLBACK
#endif

struct TessData {
    GLdouble data[6];

    TessData(glm::vec3 point, glm::vec3 color) {
        data[0] = point.x;
        data[1] = point.y;
        data[2] = point.z;
        data[3] = color.r;
        data[4] = color.g;
        data[5] = color.b;
    }
};

namespace Draw {
    enum Type{
        TypeSolid,
        TypeWireframe
    };

    static Type drawingType;

    void init();
    void end();

    // Draw a list of solids
    void draw(Solid * &solid);
    void drawPoint();
    void drawSolidLoop(Loop * &loop);
    void drawSolidFace(Face * &face);
    void drawWireFrameFace(Face * &face);
    void drawWireFrameLoop(Loop * &loop);
    // Draw one solid
    void drawSolid(Solid * &solid);

    void test();
};

#endif