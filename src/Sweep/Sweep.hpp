#ifndef SWEEP_HPP
#define SWEEP_HPP

#include "EulerOp.hpp"

class Sweep{
public:
    static Solid * sweep(Face *face, const glm::vec3 &dir, float dist);
};

#endif