#ifndef PHYSICS_H
#define PHYSICS_H

#include <Eigen/Dense>
#include "raylib.h"

typedef struct CelestialBody {
    char* name;
    Color color;

    double mass;
    float radius;

    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d force;
} CelestialBody;

#endif