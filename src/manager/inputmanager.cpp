#include "raylib.h"
#include "inputmanager.h"


void InputManager::detectInput() {
    // Toggle fullscreen
    if (IsKeyDown(KEY_F))
        ToggleFullscreen();

    // Check if the camera is enabled
    if (targetRenderer->cameraEnabled) {
        UpdateCamera(&(targetRenderer->camera), CAMERA_FREE);
    }

    // If clicked outside the UI component, enable the camera and disable the cursor
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !(targetUI->isMouseOver())) {
        targetRenderer->cameraEnabled = true;
        DisableCursor();
    }

    // Manually disable the camera and enable the cursor by pressing 'exit'
    if (IsKeyPressed(KEY_ESCAPE) && IsCursorHidden()) {
        targetRenderer->cameraEnabled = false;
        EnableCursor();
    }

    // Toggle the simulation if Spacebar is pressed
    if (IsKeyPressed(KEY_E)) {
        targetSim->isPaused = !targetSim->isPaused;
    }

    processPicking();
}


void InputManager::processPicking() {
    // Don't allow picking if the cursor is not enabled
    if (IsCursorHidden()) return;

    Ray ray = GetMouseRay(GetMousePosition(), targetRenderer->camera);
    int hoveredIndex = -1;

    for (int i = 0; i < targetSim->bodies.size(); i++) {
        Vector3 pos = { targetSim->bodies[i].position[0],
                        targetSim->bodies[i].position[1],
                        targetSim->bodies[i].position[2]
        };

        RayCollision hit = GetRayCollisionSphere(ray, pos, targetSim->bodies[i].radius);

        if (hit.hit) {
            targetRenderer->drawBodyLabel(&targetSim->bodies[i]);
            hoveredIndex = i;
            break;
        }
    }

    // Delete the planet if right clicked on the planet
    if (hoveredIndex != -1 && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        targetSim->bodies.erase(targetSim->bodies.begin() + hoveredIndex);
        return;
    }
}