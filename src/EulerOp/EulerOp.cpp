#include <iostream>
#include "EulerOp.hpp"

extern int displayEulerOp;

Solid * EulerOp::mvfs(Vertex * &v, const glm::vec3 &P){
    if(displayEulerOp){
        std::cout << "    --mvfs--" << std::endl;
    }
    v = new Vertex();
    v->cor = P;

    Solid *solid = new Solid();
    Face *face = new Face();
    Loop *loop = new Loop();
    face->add_Loop(loop);
    solid->add_Face(face);

    return solid;
}

HalfEdge * EulerOp::mev(Vertex *v1, Vertex * &v2, const glm::vec3 &P, Loop *loop){
    if(displayEulerOp){
        std::cout << "    --mev--" << std::endl;
    }
    assert(v1 != nullptr);
    assert(loop != nullptr);

    v2 = new Vertex();
    v2->cor = P;

    Edge *edge = new Edge();
    HalfEdge *he1 = new HalfEdge();
    HalfEdge *he2 = new HalfEdge();
    he1->v = v1;
    he2->v = v2;
    
    Solid *solid = loop->Lface->Fsolid;
    solid->add_Edge(edge);
    edge->connect(he1, he2);

    loop->add_HE(he1, he2);

    return he1;
}

// @Input:
//    v1, v2, loop: construct an edge from v1 to v2 in loop
// @Output:
//    newLoop: 'new loop' (inner), thus 'loop' refer to outer
Loop * EulerOp::mef(Vertex *v1, Vertex *v2, Loop *loop){
    if(displayEulerOp){
        std::cout << "    --mef--" << std::endl;
    }
    assert(v1 != nullptr);
    assert(v2 != nullptr);
    assert(loop != nullptr);

    Edge *edge = new Edge();
    HalfEdge *he1 = new HalfEdge();
    HalfEdge *he2 = new HalfEdge();
    he1->v = v1;
    he2->v = v2;

    Solid *solid = loop->Lface->Fsolid;
    solid->add_Edge(edge);
    edge->connect(he1, he2);

    HalfEdge *phe1 = loop->findHE(v1);
    HalfEdge *phe2 = loop->findHE(v2);
    assert(phe1 != nullptr);
    assert(phe2 != nullptr);

    he1->HEprev = phe1->HEprev;
    he1->HEnext = phe2;
    he2->HEprev = phe2->HEprev;
    he2->HEnext = phe1;
    phe1->HEprev->HEnext = he1;
    phe1->HEprev = he2;
    phe2->HEprev->HEnext = he2;
    phe2->HEprev = he1;

    Face * newFace = new Face();
    Loop * newLoop = new Loop();
    loop->Lhe = he2;
    newLoop->Lhe = he1;

    newFace->add_Loop(newLoop);
    solid->add_Face(newFace);

    return newLoop;
}

// @Input:
//    e: edge to be killed
//    loop: loop that e belongs to
// @Output:
//    newLoop: new loop after kill edge e, newLoop is inner
Loop * EulerOp::kemr(Edge *e, Loop *loop){
    if(displayEulerOp){
        std::cout << "    --kemr--" << std::endl;
    }
    assert(e != nullptr);
    assert(loop != nullptr);

    HalfEdge *he1 = e->he1, *he2 = e->he2;
    Face *face = loop->Lface;
    Solid *solid = face->Fsolid;
    solid->del_Edge(e);

    he1->HEprev->HEnext = he2->HEnext;
    he1->HEnext->HEprev = he2->HEprev;
    he2->HEprev->HEnext = he1->HEnext;
    he2->HEnext->HEprev = he1->HEprev;
    
    Loop *newLoop = new Loop();
    loop->Lhe = he2->HEnext;
    newLoop->Lhe = he1->HEnext;
    face->add_Loop(newLoop);

    delete he1;
    delete he2;
    return newLoop;
}

// @Input:
//    L1:
//    L2: delete F2(face that L2 belongs to), and add L2 to F1(face that L1 belongs to)
void EulerOp::kfmrh(Loop *L1, Loop *L2){
    if(displayEulerOp){
        std::cout << "    --kfmrh--" << std::endl;
    }
    assert(L1 != nullptr);
    assert(L2 != nullptr);

    Face *F1 = L1->Lface, *F2 = L2->Lface;
    Solid *solid = F1->Fsolid;
    solid->del_Face(F2);

    F1->add_Loop(L2);
}
