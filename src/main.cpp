#include "raylib.h"
#include "constants.h"

#include "visuals/visuals.h"
#include "physics/physics.h"
#include "simulation.h"


int main(void) {
    InitWindow(windowWidth, windowHeight, "Outer Space Simulation");

    // Create the camera object
    Camera3D camera = getCamera();

    // Disable the cursor and "lock" it into the window
    // DisableCursor();

    SetTargetFPS(fps);

    // Create test objects
    CelestialBody earth = CelestialBody("earth", WHITE, 100.0f, 0.25f, V::Zero());

    CelestialBody earth2 = CelestialBody("earth", WHITE, 10.0f, 0.25f, V(10, 0, 0), {0, std::sqrt(G * 100/10), 0 });

    Simulation sim = Simulation({ earth, earth2 });
    UIComponent ui = UIComponent(&sim);

    while (!WindowShouldClose()) {
        sim.update();

        BeginDrawing();

            ClearBackground(BLACK);

            UpdateCamera(&camera, CAMERA_FREE);

            BeginMode3D(camera);
                sim.draw();
            EndMode3D();

            ui.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}