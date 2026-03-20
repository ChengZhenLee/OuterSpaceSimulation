#include "raylib.h"
#include "constants.h"

#include "visuals/visuals.h"
#include "physics/physics.h"
#include "simulation.h"


int main(void) {
    InitWindow(windowWidth, windowHeight, "Outer Space Simulation");

    // Don't exit the program if 'exit' is pressed
    SetExitKey(KEY_NULL);

    // Create the camera object
    Camera3D camera = getCamera();
    bool cameraEnabled = true;
    DisableCursor();

    SetTargetFPS(fps);

    // Create the simulation
    Simulation sim = Simulation({});

    // Create the UI component
    UIComponent ui = UIComponent(&sim);

    while (!WindowShouldClose()) {
        // Toggle fullscreen
        if (IsKeyDown(KEY_F))
            ToggleFullscreen();

        // Check if the camera is enabled
        if (cameraEnabled) {
            UpdateCamera(&camera, CAMERA_FREE);
        }

        // If clicked outside the UI component, enable the camera and disable the cursor
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ui.isMouseOver()) {
            cameraEnabled = true;
            DisableCursor();
        }

        // Manually disable the camera and enable the cursor by pressing 'exit'
        if (IsKeyPressed(KEY_ESCAPE)) {
            cameraEnabled = false;
            EnableCursor();
        }

        sim.update();

        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);
                sim.draw();
            EndMode3D();

            ui.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}