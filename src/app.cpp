#include "app.h"


Camera3D App::getCamera() {
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 20.0f, 20.0f, 20.0f }; // Camera location
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Looking at the center
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // "Up" is the Y axis
    camera.fovy = 45.0f;                                // Field of view
    camera.projection = CAMERA_PERSPECTIVE;

    return camera;
}


void App::run() {
    while (!WindowShouldClose()) {
        state.fps = GetFPS();
        state.realTimeDelta = GetFrameTime();

        // Detect if there are any pending spawns
        if (state.newBody.has_value()) {
            // Add the new body into the simulation
            sim.addBody(*state.newBody);

            // Reset the newBody temporary space
            state.newBody = std::nullopt;
        }

        // Detect if there are any pending deletes
        if (state.bodyToDelete != nullptr) {
            // Delete the body from the simulation
            sim.deleteBody(state.bodyToDelete);

            // Reset the hoveredBody pointer if it's the same as the deleted body's pointer
            if (state.hoveredBody == state.bodyToDelete) state.hoveredBody = nullptr;

            // Reset the bodyToDelete pointer
            state.bodyToDelete = nullptr;
        }

        if (state.hoveredBody != nullptr) {
            r.drawBodyLabel(state.hoveredBody, state.camera);
        }

        // Detect any inputs from users
        im.detectInput(&sim, &ui);

        // Update the simulation if not paused
        if (!(state.isPaused)) sim.update(state.simTimeDelta);

        // Render
        r.display(&sim, &ui);
    }

    CloseWindow();
}