#include "raylib.h"
#include "constants.h"
#include "visuals/visuals.h"


int main(void) {
    InitWindow(windowWidth, windowHeight, title);

    // Create the camera object
    Camera3D camera = getCamera();

    // Disable the cursor and "lock" it into the window
    DisableCursor();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        UpdateCamera(&camera, CAMERA_FREE);

        BeginMode3D(camera);
            // Test draw a sphere
            DrawSphere({0.0f, 1.0f, 1.0f}, 0.5f, WHITE);

            // Test draw a grid
            DrawGrid(10, 1.0f); 
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}