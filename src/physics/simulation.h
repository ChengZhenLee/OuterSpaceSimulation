#ifndef SIMULATION_H
#define SIMULATION_H


#include "physics.h"


class Simulation {
public:
    std::vector<std::unique_ptr<CelestialBody>> bodies;
    std::vector<V> forces;
    std::vector<double> masses;
    std::vector<double> radii;
    std::vector<V> positions;

    std::vector<CelestialBody*> bodiesToDelete;
    std::vector<std::unique_ptr<CelestialBody>> bodiesToAdd;

    void addBody(CelestialBody body);

    bool isOutOfBounds(CelestialBody* body);

    void deleteBody(CelestialBody* body);

    void update(float timeDelta);

    void clear();

private:
    void applyMerge(std::unique_ptr<CelestialBody> &a, std::unique_ptr<CelestialBody> &b);

    void handleImpact(std::unique_ptr<CelestialBody> &a, std::unique_ptr<CelestialBody> &b, double impactSpeed);

    void handleCollisions(std::vector<std::unique_ptr<CelestialBody>> &bodies);
    
    void prepareBuffers(int n);    

    void handlePending();
};


void calculateForces(
    std::vector<double> &masses, 
    std::vector<double> &radii,
    std::vector<V> &positions,
    std::vector<V> &F
);


void handleCollisions();


#endif