#ifndef SIMULATION_H
#define SIMULATION_H


#include "physics.h"


class Simulation {
public:
    Simulation(std::vector<CelestialBody> bodies={}) : 
        bodies(bodies)
    {};
    
    std::vector<CelestialBody> bodies;

    void addBody(CelestialBody body);

    bool isOutOfBounds(CelestialBody body);

    void deleteBody(CelestialBody* body);

    void update(float timeDelta);

    void clear();
};


#endif