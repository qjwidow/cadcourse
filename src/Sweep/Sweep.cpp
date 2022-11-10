#include "Sweep.hpp"

Solid * Sweep::sweep(Face *face, const glm::vec3 &dir, float dist){
    Solid *solid = face->Fsolid;

    for(Loop *loop=face->Floops; loop!=nullptr; loop=loop->Lnext){
        HalfEdge *he = loop->Lhe;
        Vertex *firstV = he->v, *firstV_up;
        glm::vec3 firstV_up_cor = firstV->cor + dir * dist;
        EulerOp::mev(firstV, firstV_up, firstV_up_cor, loop);
        Vertex *last_up = firstV_up;

        he = he->HEnext;
        Vertex *now = he->v, *now_up;
        while(now != firstV){
            glm::vec3 now_up_cor = now->cor + dir * dist;
            EulerOp::mev(now, now_up, now_up_cor, loop);
            EulerOp::mef(now_up, last_up, loop);
            last_up = now_up;
            he = he->HEnext;
            now = he->v;
        }
        EulerOp::mef(firstV_up, last_up, loop);
    }

    return solid;
}