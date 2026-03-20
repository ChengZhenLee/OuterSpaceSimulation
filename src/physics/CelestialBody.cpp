#include "raylib.h"
#include "physics.h"


void CelestialBody::draw() {
    Vector3 visualPosition = { position[0], position[1], position[2] };

    DrawSphere(visualPosition, radius, WHITE);
}
