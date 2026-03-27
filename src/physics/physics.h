#ifndef PHYSICS_H
#define PHYSICS_H

#include <Eigen/Dense>
#include "celestialBody.h"
#include "constants.h"


double resolveCollision(CelestialBody &a, CelestialBody &b, V relativePos, double distance);

std::vector<std::unique_ptr<CelestialBody>> shatterBody(CelestialBody body, double impactSpeed);

float getPotentialHeight(float x, float z, std::vector<double>& masses, std::vector<V>& positions);

#endif