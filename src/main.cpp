#include "raylib.h"
#include "constants.h"
#include "visuals/visuals.h"

// test
#include "physics/physics.h"


int main(void) {
    InitWindow(windowWidth, windowHeight, "Outer Space Simulation");

    // Create the camera object
    Camera3D camera = getCamera();

    // Disable the cursor and "lock" it into the window
    DisableCursor();

    SetTargetFPS(60);

    CelestialBody earth = {
        "earth", WHITE, 1000.0f, 0.25f,
        Eigen::Matrix<float, 3, 1>::Zero(),
        Eigen::Matrix<float, 3, 1>::Zero(),
        Eigen::Matrix<float, 3, 1>::Zero(),
    };

    CelestialBody earth2 = {
        "earth2", WHITE, 10.0f, 0.25f,
        Eigen::Matrix<float, 3, 1>(10, 0, 0),
        Eigen::Matrix<float, 3, 1>(0, std::sqrt(G * 1000/10), 0),
        Eigen::Matrix<float, 3, 1>::Zero(),
    };

    std::vector<CelestialBody> bodies = { earth, earth2 };

    while (!WindowShouldClose()) {
        updateBodies(bodies);

        BeginDrawing();

        ClearBackground(BLACK);

        UpdateCamera(&camera, CAMERA_FREE);

        BeginMode3D(camera);
            // Test draw a sphere
            for (CelestialBody body:bodies) {
                Vector3 position = { body.position[0], body.position[1], body.position[2] };
                DrawSphere(position, body.radius, WHITE);
            }

            // Test draw a grid
            DrawGrid(1000, 1.0f); 
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}