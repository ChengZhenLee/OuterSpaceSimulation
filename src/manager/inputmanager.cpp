#include "raylib.h"
#include "inputmanager.h"
#include "physics/physics.h"
#include "visuals/visuals.h"


void InputManager::detectInput(Simulation* sim, UIComponent* ui) {
    // Toggle fullscreen
    if (IsKeyDown(KEY_F)) ToggleFullscreen();

    // Check if the camera is enabled (cursor is locked)
    if (state->cursorLocked) UpdateCamera(&(state->camera), CAMERA_FREE);

    // If clicked outside the UI component, enable the camera and disable the cursor
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !(ui->isMouseOver())) {
        state->cursorLocked = true;
        DisableCursor();
    }

    // Manually disable the camera and enable the cursor by pressing 'exit'
    if (IsKeyPressed(KEY_ESCAPE) && IsCursorHidden()) {
        state->cursorLocked = false;
        EnableCursor();
    }

    // Toggle the simulation if Spacebar is pressed
    if (IsKeyPressed(KEY_E)) state->isPaused = !(state->isPaused);

    // Picking logic to pick bodies
    processPicking(sim);
}


void InputManager::processPicking(Simulation* sim) {
    // Reset the hover state every frame
    state->hoveredBody = nullptr;

    // Don't allow picking if the cursor is locked (flying around with the camera)
    if (state->cursorLocked) return;

    Ray ray = GetMouseRay(GetMousePosition(), state->camera);

    for (auto& body : sim->bodies) {
        Vector3 pos = { body.position[0], body.position[1], body.position[2] };
        RayCollision hit = GetRayCollisionSphere(ray, pos, body.radius);

        if (hit.hit) {
            state->hoveredBody = &body;

            // Notify that this body is to be deleted if right mouse button is clicked while hovering
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                state->bodyToDelete = &body;
            }
            break;
        }
    }
}