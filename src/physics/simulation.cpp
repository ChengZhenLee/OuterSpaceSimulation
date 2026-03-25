#include "constants.h"
#include "simulation.h"
#include "physics.h"


bool Simulation::isOutOfBounds(CelestialBody* body) {
    for (int i = 0; i < 3; i++) {
        if (std::abs(body->position[i]) > limit)
            return true;
    }

    return false;
}


void Simulation::addBody(CelestialBody body) {
    bodies.push_back(std::make_unique<CelestialBody>(body));
}


void Simulation::deleteBody(CelestialBody* body) {
    if (body == nullptr) return;

    // Erase the body from the std::vector
    std::erase_if(bodies, [body](const std::unique_ptr<CelestialBody>& b) {
        return b.get() == body;
    });
}


void Simulation::clear() {
    bodies.clear();
}


void Simulation::handlePending() {
    for (CelestialBody* body : bodiesToDelete) { 
        deleteBody(body);
    }
    bodiesToDelete.clear();

    for (const auto& body : bodiesToAdd) {
        addBody(std::move(*body));
    }
    bodiesToAdd.clear();
}


void Simulation::handleCollisions(std::vector<std::unique_ptr<CelestialBody>> &bodies){
    for(int i = 0; i < bodies.size(); i++){
        for(int j = i+1; j < bodies.size(); j++){
            //vector that points from center of Planet A to center of planet B
            V relativePos = bodies[j]->position - bodies[i]->position;

            double distance = relativePos.norm();

            double minDistance = bodies[i]->radius + bodies[j]->radius;

            if(distance < minDistance){
                // if theres a collision, update the velocities and forces so they do not overlap
                double impactSpeed = resolveCollision(*bodies[i], *bodies[j], relativePos, distance);

                double threshold = 0.001 * (1.0 + log10(bodies[i]->mass + bodies[j]->mass));

                // Check the behaviour of collision
                if (impactSpeed > threshold) {
                    double massRatio = bodies[i]->mass / bodies[j]->mass;

                    // Body i is much larger than j -> merge j into i
                    if (massRatio > 100.0) {
                        V combinedMomentum = bodies[i]->mass * bodies[i]->velocity + bodies[j]->mass * bodies[j]->velocity;
                        bodies[i]->mass += bodies[j]->mass;

                        // Recalculate the velocity (inelastic collision)
                        bodies[i]->velocity = combinedMomentum / bodies[i]->mass;

                        // Delete the "eaten" body
                        bodiesToDelete.push_back(bodies[j].get());
                    }
                    // Body j is much larger than body i -> merge i into j
                    else if (massRatio < 0.01) {
                        V combinedMomentum = bodies[i]->mass * bodies[i]->velocity + bodies[j]->mass * bodies[j]->velocity;
                        bodies[j]->mass += bodies[i]->mass;

                        // Recalculate the velocity (inelastic collision)
                        bodies[j]->velocity = combinedMomentum / bodies[j]->mass;

                        // Delete the "eaten" body
                        bodiesToDelete.push_back(bodies[i].get());
                    }
                    // If the masses are almost equal, shatter both bodies
                    else {
                        auto fragsA = shatterBody(*bodies[i], impactSpeed);
                        auto fragsB = shatterBody(*bodies[j], impactSpeed);

                        // Delete the original bodies
                        bodiesToDelete.push_back(bodies[i].get());
                        bodiesToDelete.push_back(bodies[j].get());

                        bodiesToAdd.insert(bodiesToAdd.end(), 
                                        std::make_move_iterator(fragsA.begin()), 
                                        std::make_move_iterator(fragsA.end()));
                        bodiesToAdd.insert(bodiesToAdd.end(), 
                                        std::make_move_iterator(fragsB.begin()), 
                                        std::make_move_iterator(fragsB.end()));
                    }
                }
            }
        }
    }
}


void Simulation::update(float timeDelta) {
    // Remove out of bounds bodies
    for (auto& body : bodies) {
        if (isOutOfBounds(body.get())) {
            deleteBody(body.get());
        }
    }

    float dt = timeDelta / substepCount;

    for (int s = 0; s < substepCount; s++){
        int n = bodies.size();
        if (n == 0) break;

        prepareBuffers(n);

        calculateForces(masses, radii, positions, forces);

        for (int i = 0; i < n; i++) {
            V acceleration = forces[i] / masses[i];
            bodies[i]->velocity += acceleration * dt;   // Update speed based on current gravity
            bodies[i]->position += bodies[i]->velocity * dt; // Move using the NEW speed

            positions[i] = bodies[i]->position; // Sync for collision check
        }

        handleCollisions(bodies);
        handlePending();
    }
}


void Simulation::prepareBuffers(int n) {
    if (forces.size() != n) {
        forces.resize(n);
        masses.resize(n);
        positions.resize(n);
        radii.resize(n);
    }

    for (int i = 0; i < n; i++) {
        masses[i] = bodies[i]->mass;
        positions[i] = bodies[i]->position;
        radii[i] = bodies[i]->radius;
    }
}