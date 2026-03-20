#include "visuals.h"
#include "../physics/physics.h"


void Renderer::setCamera() {
    camera = { 0 };
    camera.position = (Vector3){ 20.0f, 20.0f, 20.0f }; // Camera location
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Looking at the center
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // "Up" is the Y axis
    camera.fovy = 45.0f;                                // Field of view
    camera.projection = CAMERA_PERSPECTIVE;
}


void Renderer::display() {
    BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);
            targetSim->draw();
        EndMode3D();

        targetUI->draw();

    EndDrawing();
}


void Renderer::drawBodyLabel(CelestialBody* body) {
    Vector3 worldPosition = { body->position[0], body->position[1], body->position[2] };

    // Offset the y coordinate a little
    worldPosition.x -= body->radius;
    worldPosition.y += body->radius + 1.0f;

    Vector2 screenPosition = GetWorldToScreen(worldPosition, camera);

    DrawText(body->name.c_str(), screenPosition.x, screenPosition.y, 20, BLUE);
}