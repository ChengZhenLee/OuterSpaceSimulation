#include "simulation.h"
#include "physics/physics.h"
#include "visuals/visuals.h"


void Simulation::addBody(CelestialBody body) {
    bodies.push_back(body);
}

void Simulation::update() {
    if (!isPaused) {
        updateBodies(bodies);
    }
}

void Simulation::draw() {
    for (CelestialBody body:bodies) {
        body.draw();
    }
    drawGravityGrid(bodies);
}