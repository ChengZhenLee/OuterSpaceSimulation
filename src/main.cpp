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
        "earth", WHITE, 10.0f, 0.25f,
        Eigen::Matrix<float, 3, 1>::Zero(),
        Eigen::Matrix<float, 3, 1>::Zero(),
        Eigen::Matrix<float, 3, 1>::Zero(),
    };

    CelestialBody earth2 = {
        "earth2", WHITE, 10.0f, 0.25f,
        Eigen::Matrix<float, 3, 1>(10, 0, 0),
        Eigen::Matrix<float, 3, 1>(0, std::sqrt(G * 10/10), 0),
        Eigen::Matrix<float, 3, 1>::Zero(),
    };

    std::vector<CelestialBody> bodies = { earth, earth2 };

    while (!WindowShouldClose()) {
        updateBodies(bodies);

        BeginDrawing();

        ClearBackground(BLACK);

        UpdateCamera(&camera, CAMERA_FREE);

        std::vector<double> masses = { earth.mass, earth2.mass };
        std::vector<Eigen::Matrix<float, 3, 1>> positions(2);
        BeginMode3D(camera);
            // Test draw a sphere
            for (int i = 0; i < 2; i++) {
                Vector3 position = { bodies[i].position[0], bodies[i].position[1], bodies[i].position[2] };
                positions[i] = bodies[i].position;

                DrawSphere(position, bodies[i].radius, WHITE);
            }

            // Test draw a grid
            DrawGravityGrid(masses, positions);
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}