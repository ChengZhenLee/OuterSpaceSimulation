#include "raylib.h"
#include "constants.h"
#include "visuals/visuals.h"

// test
#include "physics/physics.h"
#include "simulation.h"


int main(void) {
    InitWindow(windowWidth, windowHeight, "Outer Space Simulation");

    // Create the camera object
    Camera3D camera = getCamera();

    // Disable the cursor and "lock" it into the window
    DisableCursor();

    SetTargetFPS(fps);

    // Create test objects
    CelestialBody earth = {
        "earth", WHITE, 100.0f, 0.25f,
        Eigen::Matrix<float, 3, 1>::Zero(),
        Eigen::Matrix<float, 3, 1>::Zero(),
        Eigen::Matrix<float, 3, 1>::Zero(),
    };

    CelestialBody earth2 = {
        "earth2", WHITE, 10.0f, 0.25f,
        Eigen::Matrix<float, 3, 1>(10, 0, 0),
        Eigen::Matrix<float, 3, 1>(0, std::sqrt(G * 100/10), 0),
        Eigen::Matrix<float, 3, 1>::Zero(),
    };

    UIComponent ui = {};
    Simulation sim = {
        camera,
        { earth, earth2 },
        ui
    };

    while (!WindowShouldClose()) {
        sim.update();

        BeginDrawing();

            ClearBackground(BLACK);

            UpdateCamera(&camera, CAMERA_FREE);

            sim.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}