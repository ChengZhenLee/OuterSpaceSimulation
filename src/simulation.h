#ifndef SIMULATION_H
#define SIMULATION_H


#include <iostream>
#include "physics/physics.h"
#include "visuals/visuals.h"


class Simulation {
    public:
        Camera &camera;
        std::vector<CelestialBody> bodies;
        UIComponent ui;

        void addBody(CelestialBody body) {
            bodies.push_back(body);
        }

        void update() {
            updateBodies(bodies);
        }

        void draw() {
            BeginMode3D(camera); 
                for (CelestialBody body:bodies) {
                    body.draw();
                }
                drawGravityGrid(bodies);
            EndMode3D();
            
            ui.draw();
        }
};


#endif