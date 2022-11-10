#include <iostream>
#include <glm/glm.hpp>

#include "EulerOp.hpp"
#include "Sweep.hpp"
#include "Draw.hpp"

int displayEulerOp = 1; // 1: display each Euler Operation, 0: otherwise

Solid * one_hole(){
    Solid *solid;
    Vertex *v0[3];
    solid = EulerOp::mvfs(v0[0], glm::vec3(0.0, 0.0, 0.0));
    Face *topFace = solid->Sfaces;
    Loop *top = topFace->Floops;

    EulerOp::mev(v0[0], v0[1], glm::vec3(2.0, 0.0, 0.0), top);
    EulerOp::mev(v0[1], v0[2], glm::vec3(1.0, 0.0, 1.732), top);
    Loop *bottom = EulerOp::mef(v0[2], v0[0], top);

    Vertex *v1[4];
    HalfEdge *tempEdge = EulerOp::mev(v0[0], v1[0], glm::vec3(0.75, 0.0, 0.5), top);
    EulerOp::mev(v1[0], v1[1], glm::vec3(0.75, 0.0, 0.9), top);
    EulerOp::mev(v1[1], v1[2], glm::vec3(1.25, 0.0, 0.9), top);
    EulerOp::mev(v1[2], v1[3], glm::vec3(1.25, 0.0, 0.5), top);
    Loop *hole_bottom = EulerOp::mef(v1[3], v1[0], top);
    hole_bottom->inner = true;
    EulerOp::kfmrh(bottom, hole_bottom);

    Loop *hole_top = EulerOp::kemr(tempEdge->HEedge, top);
    hole_top->inner = true;

    solid = Sweep::sweep(topFace, glm::vec3(0.0, 1.0, 0.0), 1.0);
    return solid;
}
int main(int argc, char **argv){
    Solid *solid;
    solid = one_hole();
    
    // shading
    Draw::init();
    Draw::draw(solid);
    Draw::end();

    return 0;
}