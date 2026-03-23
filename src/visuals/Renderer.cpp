#include "raylib.h"
#include <algorithm>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "visuals.h"
#include "physics/physics.h"
#include "types.h"


void Renderer::drawGravityGrid(std::vector<CelestialBody> &bodies) {
    int length = bodies.size();
    std::vector<double> masses(length);
    std::vector<V> positions(length);

    for (int i = 0; i < length; i++) {
        masses[i] = bodies[i].mass;
        positions[i] = bodies[i].position;
    }

    float distanceBetweenGridpoints = 1.0f; // Grid spacing
    int size = gridSize;
    int gridCount = (size * 2) + 1; // total points in the grid

    //calculate y positons first(2D vector) 
    std::vector<std::vector<float>>heightGrid(gridCount, std::vector<float>(gridCount));
    
    for(int i = 0; i <gridCount; i++){
        // calculate x positions
        float x = (i - size) * distanceBetweenGridpoints;
        for(int j = 0; j < gridCount; j++){
            //calculate z positions
            float z = (j - size) * distanceBetweenGridpoints;
            //calculate height once per point
            heightGrid[i][j] = getPotentialHeight(x, z, masses, positions);
        }
    }
    for(int i = 0; i < gridCount; i++){
        float x = (i - size) * distanceBetweenGridpoints;
        for(int j = 0; j < gridCount; j++){
            
            float z = (j - size) * distanceBetweenGridpoints;
            float y = heightGrid[i][j];
            Vector3 p1 ={x, y ,z};
            
            //connect to right neighbor
            if(i < gridCount - 1){
                float yRight = heightGrid[i+1][j];
                DrawLine3D(p1, {x + distanceBetweenGridpoints, yRight, z}, DARKGRAY);
            }

            //connect to front neighbor
            if(j < gridCount - 1){
                float yFront = heightGrid[i][j+1];
                DrawLine3D(p1, {x, yFront, z + distanceBetweenGridpoints}, DARKGRAY);
            }
        }
    }
}


void Renderer::drawCelestialBody(CelestialBody& body) {
    Vector3 pos = { (float)body.position[0], (float)body.position[1], (float)body.position[2] };

    // Drawing the actual scaled size of the celestial body
    double visualRadius = 0.05 + log10f(body.radius * 100000.0) * 0.1;
    DrawSphere(pos, visualRadius, body.color);

    // Drawing the halo of the celestial body
    double haloRadius = std::max(MIN_VISUAL_RADIUS, visualRadius);
    Color haloColor = ColorAlpha(body.color, HALO_OPACITY);
    DrawSphere(pos, haloRadius, haloColor);
    DrawSphereWires(pos, haloRadius, 8, 8, ColorAlpha(body.color, 0.5f));
}


void Renderer::display(Simulation* sim, UIComponent* ui) {
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(state->camera);

            // Draw each celestial body 
            for (auto body : sim->bodies) {
                drawCelestialBody(body);
                drawBodyLabel(&body, state->camera);
            }
            // Draw the grid
            drawGravityGrid(sim->bodies);

        EndMode3D();

        // Draw the frame rate and time scale
        DrawText(TextFormat("%s fps", std::to_string((int)state->fps).c_str()), windowWidth - 200.0f, 0.0f, 20, WHITE);
        DrawText(TextFormat("%sxSpeed", ui->timeScaleText), windowWidth - 200.0f, 30.0f, 20, WHITE);
        if (state->isPaused) {
            DrawText("Paused", windowWidth - 200.0f, 60.0f, 20, RED);
        } else {
            DrawText("Running", windowWidth - 200.0f, 60.0f, 20, GREEN);
        }

        // Draw the ui
        drawUI(ui);

    EndDrawing();
}


void Renderer::drawBodyLabel(CelestialBody* body, Camera3D& camera) {
    Vector3 worldPosition = { (float)body->position[0], (float)body->position[1], (float)body->position[2] };

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

    // Name
    GuiLabel({ 30, 70, 100, 20 }, "Name");
    if (GuiTextBox({ 30, 90, 160, 30 }, ui->nameText, 64, ui->editNameMode))
        ui->editNameMode = !(ui->editNameMode);

    // Mass
    GuiLabel({ 30, 130, 100, 20 }, "Mass (in Earths)");
    if (GuiTextBox({ 30, 150, 160, 30 }, ui->massText, 64, ui->editMassMode))
        ui->editMassMode = !(ui->editMassMode);

   // Position
    GuiLabel({ 30, 190, 100, 20 }, "Position (X, Y, Z)");
    // X
    if (GuiTextBox({ 30, 210, 50, 30 }, ui->positionText[0], 64, ui->editPositionMode[0])) 
        ui->editPositionMode[0] = !(ui->editPositionMode[0]);
    // Y
    if (GuiTextBox({ 85, 210, 50, 30 }, ui->positionText[1], 64, ui->editPositionMode[1])) 
        ui->editPositionMode[1] = !(ui->editPositionMode[1]);
    // Z
    if (GuiTextBox({ 140, 210, 50, 30 }, ui->positionText[2], 64, ui->editPositionMode[2])) 
        ui->editPositionMode[2] = !(ui->editPositionMode[2]);

    // Radius
    GuiLabel({ 30, 250, 100, 20 }, "Radius (km)");
    if (GuiTextBox({ 140, 270, 50, 30 }, ui->radiusText, 64, ui->editRadiusMode))
        ui->editRadiusMode = !(ui->editRadiusMode);

    // Spawn Celestial Body
    if (GuiButton({ 30, 310, 160, 30 }, "Spawn Planet")) {
        ui->setNewBody();
    }

    // Edit the time scale
    GuiLabel({ 30, 350, 100, 20 }, "Time Scale");
    if (GuiTextBox({ 140, 370, 50, 30 }, ui->timeScaleText, 64, ui->editTimeScaleMode)) {
        ui->editTimeScaleMode = !(ui->editTimeScaleMode);
        
        if (!(ui->editTimeScaleMode)) {
            ui->setTimeScale();
        }
    }

    // Pause simulation
    if (GuiButton({ 30, 410, 160, 30 }, "Toggle Simulation (E)"))
        state->isPaused = !(state->isPaused);

    // Clear simulation
    if(GuiButton({ 30, 450, 160, 30 }, "Clear Simulation"))
        state->clear = true;

    // Close the editor
    if (GuiButton({ 30, 490, 160, 30 }, "Close Editor")) 
        state->showMenu = false;
}