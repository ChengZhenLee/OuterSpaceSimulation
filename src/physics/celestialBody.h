#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H


#include <deque>
#include "constants.h"
#include "raylib.h"
#include <Eigen/Dense>


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
    std::deque<V> trail;
    std::string name;
    Color color;

    // Update the trail
    void updateTrail() {
        trail.push_front(position);
        if (trail.size() > MAXTRAILLENGTH) trail.pop_back();
    }
};

#endif