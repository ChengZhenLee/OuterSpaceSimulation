#ifndef PHYSICS_H
#define PHYSICS_H

#include <Eigen/Dense>
#include "constants.h"
#include "raylib.h"


using V = Eigen::Matrix<double, 3, 1>;

class CelestialBody {
public:
    // Constructor
    CelestialBody(std::string name, Color color, double massInEarths, double radiusInKm, 
        V position, V velocity={ 0.0, 0.0, 0.0 }, V force={ 0.0, 0.0, 0.0 }) :
            name(name),
            color(color),
            mass(massInEarths),
            radius(radiusInKm),
            position(position),
            velocity(velocity),
            force(force)
    {};

    // Simulation variables
    double mass;
    double radius;
    V position;
    V velocity;
    V force;

    // Visual variables
    std::string name;
    Color color;
};


void calculateForces();

double resolveCollision(CelestialBody &a, CelestialBody &b, V relativePos, double distance);

std::vector<std::unique_ptr<CelestialBody>> shatterBody(CelestialBody body, double impactSpeed);

float getPotentialHeight(float x, float z, std::vector<double>& masses, std::vector<V>& positions);

#endif