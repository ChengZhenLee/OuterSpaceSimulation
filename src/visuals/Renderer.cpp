#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "visuals.h"
#include "../physics/physics.h"
#include "../types.h"


void Renderer::display(Simulation* sim, UIComponent* ui) {
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(state->camera);

            // Draw each celestial body
            for (auto body : sim->bodies) {
                DrawSphere({ body.position[0], body.position[1], body.position[2] }, body.radius, body.color);
                drawBodyLabel(&body, state->camera);

            }
            // Draw the grid
            drawGravityGrid(sim->bodies);

        EndMode3D();

        // Draw the ui
        drawUI(ui);

    EndDrawing();
}


void Renderer::drawBodyLabel(CelestialBody* body, Camera3D camera) {
    Vector3 worldPosition = { body->position[0], body->position[1], body->position[2] };

    // Offset the y coordinate a little
    worldPosition.x -= body->radius;
    worldPosition.y += body->radius + 1.0f;

    Vector2 screenPosition = GetWorldToScreen(worldPosition, camera);

    DrawText(body->name.c_str(), screenPosition.x, screenPosition.y, 20, BLUE);
}


void Renderer::drawUI(UIComponent* ui) {
    if (!state->showMenu) {
        if (GuiButton({ 20, 20, 160, 30 }, "Open Editor")) 
            state->showMenu = true;
        return;
    }

    GuiGroupBox(ui->panelRect, "Create Celestial Body");

    // We use the buffers from the UI object
    if (GuiTextBox({ 30, 70, 160, 30 }, ui->nameText, 64, ui->editNameMode))
        ui->editNameMode = !(ui->editNameMode);

    if (GuiTextBox({ 30, 120, 160, 30 }, ui->massText, 64, ui->editMassMode))
        ui->editMassMode = !(ui->editMassMode);

   // Position
    GuiLabel({ 30, 160, 100, 20 }, "Position (X, Y, Z)");
    // X
    if (GuiTextBox({ 30, 180, 50, 30 }, ui->positionText[0], 64, ui->editPositionMode[0])) 
        ui->editPositionMode[0] = !(ui->editPositionMode[0]);
    // Y
    if (GuiTextBox({ 85, 180, 50, 30 }, ui->positionText[1], 64, ui->editPositionMode[1])) 
        ui->editPositionMode[1] = !(ui->editPositionMode[1]);
    // Z
    if (GuiTextBox({ 140, 180, 50, 30 }, ui->positionText[2], 64, ui->editPositionMode[2])) 
        ui->editPositionMode[2] = !(ui->editPositionMode[2]);

    // Radius
    GuiLabel({ 30, 220, 100, 20 }, "Radius");
    if (GuiTextBox({ 140, 240, 50, 30 }, ui->radiusText, 64, ui->editRadiusMode))
        ui->editRadiusMode = !(ui->editRadiusMode);

    // Spawn Planet
    if (GuiButton({ 30, 280, 160, 30 }, "Spawn Planet")) {
        ui->setNewBody();
    }
    
    // Pause simulation
    if (GuiButton({ 30, 320, 160, 30 }, "Toggle Simulation"))
        state->isPaused = !(state->isPaused);

    // Close the editor
    if (GuiButton({ 30, 360, 160, 30 }, "Close Editor")) 
        state->showMenu = false;
}