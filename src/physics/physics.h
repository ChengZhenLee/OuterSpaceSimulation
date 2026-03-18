#ifndef PHYSICS_H
#define PHYSICS_H

#include <Eigen/Dense>
#include "raylib.h"

typedef struct CelestialBody {
    char* name;
    Color color;

    double mass;
    float radius;

    Eigen::Matrix<float, 3, 1> position;
    Eigen::Matrix<float, 3, 1> velocity;
    Eigen::Matrix<float, 3, 1> force;
} CelestialBody;

#endif