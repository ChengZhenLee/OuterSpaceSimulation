#ifndef PHYSICS_H
#define PHYSICS_H

#include <Eigen/Dense>
#include "raylib.h"

typedef struct CelestialBody {
    std::string name;
    Color color;

    double mass;
    float radius;

    Eigen::Matrix<float, 3, 1> position;
    Eigen::Matrix<float, 3, 1> velocity;
    Eigen::Matrix<float, 3, 1> force;
} CelestialBody;

void updateBodies(std::vector<CelestialBody> &bodies);


float getPotentialHeight(float x, float z, const std::vector<double>& masses, const std::vector<Eigen::Matrix<float, 3, 1>>& positions);
#endif