#include "constants.h"
#include "simulation.h"
#include "physics.h"


void Simulation::addBody(CelestialBody body) {
    bodies.push_back(body);
}

bool Simulation::isOutOfBounds(CelestialBody body) {
    for (int i = 0; i < 3; i++) {
        if (std::abs(body.position[i]) > limit)
            return true;
    }

    return false;
}

void Simulation::deleteBody(CelestialBody* body) {
    if (body == nullptr) return;

    // Erase the body from the std::vector
    std::erase_if(bodies, [body](const CelestialBody& b) {
        return &b == body;
    });
}

void Simulation::update(float timeDelta) {
    // Remove out of bounds bodies
    for (CelestialBody& body : bodies) {
        if (isOutOfBounds(body)) {
            deleteBody(&body);
        }
    }

    updateBodies(bodies, timeDelta);
}


void Simulation::clear() {
    bodies.clear();
}