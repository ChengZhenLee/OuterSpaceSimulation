#ifndef PHYSICS_H
#define PHYSICS_H

#include <Eigen/Dense>
#include "raylib.h"


using V = Eigen::Matrix<float, 3, 1>;

class CelestialBody {
public:
    // Constructor
    CelestialBody(std::string name, Color color, double mass, float radius, 
        V position, V velocity={ 0.0f, 0.0f, 0.0f }, V force={ 0.0f, 0.0f, 0.0f }) :
            name(name),
            color(color),
            mass(mass),
            radius(radius),
            position(position),
            velocity(velocity),
            force(force)
    {};

    std::string name;
    Color color;
    double mass;
    float radius;
    V position;
    V velocity;
    V force;
};


void updateBodies(std::vector<CelestialBody> &bodies);


float getPotentialHeight(float x, float z, std::vector<double>& masses, std::vector<V>& positions);
#endif