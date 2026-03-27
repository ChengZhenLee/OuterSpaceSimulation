#include "raylib.h"
#include "constants.h"
#include "inputmanager.h"
#include "visuals/visuals.h"


void InputManager::detectInput(Simulation* sim, UIComponent* ui) {
    // Toggle fullscreen
    if (IsKeyDown(KEY_F) && !(ui->isMouseOver())) ToggleFullscreen();

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
    if (IsKeyPressed(KEY_E) && !(ui->isMouseOver())) state->isPaused = !(state->isPaused);

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
        Vector3 pos = { (float)body->position[0], (float)body->position[1], (float)body->position[2] };
        double pickingRadius = std::max(MIN_VISUAL_RADIUS, (double)0.05f + log10f(body->radius * 100000.0) * 0.1); 
        RayCollision hit = GetRayCollisionSphere(ray, pos, pickingRadius);

        if (hit.hit) {
            state->hoveredBody = body.get();

            // Notify that this body is to be deleted if right mouse button is clicked while hovering
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                state->bodyToDelete = body.get();
            }
            break;
        }
    }
}