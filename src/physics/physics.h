#ifndef PHYSICS_H
#define PHYSICS_H

#include <Eigen/Dense>
#include "raylib.h"


using V = Eigen::Matrix<float, 3, 1>;

class CelestialBody {
    public:
        std::string title;
        Color color;
        double mass;
        float radius;
        V position;
        V velocity;
        V force;

        void draw() {
            Vector3 visualPosition = { position[0], position[1], position[2] };

            DrawSphere(visualPosition, radius, WHITE);
        }
};


void updateBodies(std::vector<CelestialBody> &bodies);


float getPotentialHeight(float x, float z, const std::vector<double>& masses, const std::vector<Eigen::Matrix<float, 3, 1>>& positions);
#endif