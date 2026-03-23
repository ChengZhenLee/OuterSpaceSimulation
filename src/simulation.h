#ifndef SIMULATION_H
#define SIMULATION_H


#include "physics/physics.h"


class Simulation {
public:
    Simulation(std::vector<CelestialBody> bodies={}) : 
        bodies(bodies)
    {};
    
    std::vector<CelestialBody> bodies;

    void addBody(CelestialBody body);

    void deleteBody(CelestialBody* body);

    void update(float timeDelta);
};


#endif