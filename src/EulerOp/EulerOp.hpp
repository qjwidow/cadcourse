#ifndef EULEROP_HPP
#define EULEROP_HPP

#include "HalfEdgeDS.hpp"

class EulerOp{
public:
    static Solid * mvfs(Vertex * &v, const glm::vec3 &P);
    static HalfEdge * mev(Vertex *v1, Vertex * &v2, const glm::vec3 &P, Loop *loop);
    static Loop * mef(Vertex *v1, Vertex *v2, Loop *loop);
    static Loop * kemr(Edge *e, Loop *loop);
    static void kfmrh(Loop *L1, Loop *L2);
};

#endif
