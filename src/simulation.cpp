#include "simulation.h"
#include "physics/physics.h"


void Simulation::addBody(CelestialBody body) {
    bodies.push_back(body);
}

void Simulation::deleteBody(CelestialBody* body) {
    if (body == nullptr) return;

    // Erase the body from the std::vector
    std::erase_if(bodies, [body](const CelestialBody& b) {
        return &b == body;
    });
}

void Simulation::update(float timeDelta) {
    updateBodies(bodies, timeDelta);
}