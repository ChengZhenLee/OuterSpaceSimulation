#ifndef SIMULATION_H
#define SIMULATION_H


#include "physics/physics.h"


class Simulation {
public:
    Simulation(std::vector<CelestialBody> bodies) : 
        bodies(bodies)
    {};
    
    std::vector<CelestialBody> bodies;

    bool isPaused = false;

    void addBody(CelestialBody body);

    void update();

    void draw();
};


#endif