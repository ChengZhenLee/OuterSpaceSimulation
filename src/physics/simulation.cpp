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


// The first body "eats" the second body
void Simulation::applyMerge(std::unique_ptr<CelestialBody> &a, std::unique_ptr<CelestialBody> &b) {
    // Combine b into a
    double totalMass = (a->mass + b->mass);
    a->mass = totalMass;
    a->velocity = (a->mass * a->velocity + b->mass * b->velocity) / totalMass;
    a->radius = pow(pow(a->radius, 3.0) + pow(b->radius, 3.0), 1.0 / 3.0);
}


void Simulation::handleImpact(std::unique_ptr<CelestialBody> &a, std::unique_ptr<CelestialBody> &b, double impactSpeed) {
    double threshold = 0.001 * (1.0 + log10(a->mass + b->mass));

    // Ratio of Big over small
    double massRatio = (a->mass > b->mass) ? a->mass / b->mass : b->mass / a->mass;

    // If they are not impacting fast, then merge the bodies
    if (impactSpeed < threshold) {
        if (a->mass > b->mass) {
            // A eats B
            bodiesToDelete.push_back(b.get());
            applyMerge(a, b);
        }
        else {
            // B eats A
            bodiesToDelete.push_back(a.get());
            applyMerge(b, a);
        }
    }

    // Partial Shatter
    else if (impactSpeed >= threshold && massRatio > 10.0) {
        std::vector<std::unique_ptr<CelestialBody>> fragments;
        if (a->mass > b->mass) {
            fragments = shatterBody(*b, impactSpeed);
            bodiesToDelete.push_back(b.get());

            // Increase the mass of the larger a little depending
            // if there are fragments or not
            if (fragments.empty()) a->mass += b->mass;
            else a->mass += b->mass * 0.5;
        } else {
            fragments = shatterBody(*a, impactSpeed);
            bodiesToDelete.push_back(a.get());

            if (fragments.empty()) b->mass += a->mass;
            else b->mass += a->mass * 0.5;
        }
        bodiesToAdd.insert(bodiesToAdd.end(), 
                            std::make_move_iterator(fragments.begin()), 
                            std::make_move_iterator(fragments.end()));
    }

    // Shatter both if the impact speed is large enough and the mass ratio is large
    else {
        auto fragsA = shatterBody(*a, impactSpeed);
        auto fragsB = shatterBody(*b, impactSpeed);

        bodiesToDelete.push_back(a.get());
        bodiesToDelete.push_back(b.get());

        bodiesToAdd.insert(bodiesToAdd.end(), 
                        std::make_move_iterator(fragsA.begin()), 
                        std::make_move_iterator(fragsA.end()));
        bodiesToAdd.insert(bodiesToAdd.end(), 
                         std::make_move_iterator(fragsB.begin()), 
                           std::make_move_iterator(fragsB.end()));
    }
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

                handleImpact(bodies[i], bodies[j], impactSpeed);
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
    }
    handleCollisions(bodies);
    handlePending();
}