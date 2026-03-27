#ifndef SIMULATION_H
#define SIMULATION_H


#include "explosionParticle.h"
#include "physics.h"
#include "celestialBody.h"
#include "constants.h"


class Simulation {
public:
    std::vector<std::unique_ptr<ExplosionParticle>> particles;
    std::vector<std::unique_ptr<CelestialBody>> bodies;
    std::vector<V> forces;
    std::vector<double> masses;
    std::vector<double> radii;
    std::vector<V> positions;

    std::vector<CelestialBody*> bodiesToDelete;
    std::vector<std::unique_ptr<CelestialBody>> bodiesToAdd;

    std::vector<ExplosionParticle*> particlesToDelete;
    std::vector<std::unique_ptr<ExplosionParticle>> particlesToAdd;

    void addBody(CelestialBody body);

    bool isOutOfBounds(CelestialBody* body);

    void deleteBody(CelestialBody* body);

    void update(float timeDelta);

    void clear();

private:
    void createExplosion(V position, double impactSpeed, Color color);

    void handleExplosion(float timeDelta);

    void deleteParticle(ExplosionParticle* particle);

    void addParticle(ExplosionParticle particle);

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