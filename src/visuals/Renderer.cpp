#include "raylib.h"
#include <algorithm>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "constants.h"
#include "visuals.h"
#include "types.h"


void Renderer::drawGravityGrid(std::vector<std::unique_ptr<CelestialBody>> &bodies) {
    int length = bodies.size();
    std::vector<double> masses(length);
    std::vector<V> positions(length);

    for (int i = 0; i < length; i++) {
        masses[i] = bodies[i]->mass;
        positions[i] = bodies[i]->position;
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


void Renderer::drawParticleTrail(ExplosionParticle& particle) {
    float length = particle.trail.size();

    for (float i = 0; i < length-1; i++) {
        Vector3 startPoint = { (float)particle.trail[i][0], (float)particle.trail[i][1], (float)particle.trail[i][2] };
        Vector3 endPoint = { (float)particle.trail[i+1][0], (float)particle.trail[i+1][1], (float)particle.trail[i+1][2] };

        DrawLine3D(startPoint, endPoint, particle.color);
    }
}


void Renderer::drawParticle(ExplosionParticle& particle) {
    Vector3 pos = { (float)particle.position[0], (float)particle.position[1], (float)particle.position[2] };

    // Draw the particle
    DrawSphere(pos, particle.radius, particle.color);
}


void Renderer::drawBodyTrail(CelestialBody& body) {
    float length = body.trail.size();

    // Get the opposite of the planet's direction
    float speed = body.velocity.norm();
    V oppDir;
    if (speed != 0) oppDir = body.velocity / speed * -1;
    
    // Get the offset for each trail-point position
    V offset = oppDir * body.radius * 0.9;

    for (float i; i < length - 1; i++) {
        // Colour fades
        float alpha = 1.0 - i / std::max(length, 1.0f);

        // Radius decreases in size
        float radiusStart = body.radius * 0.4f * alpha;
        float radiusEnd = body.radius * 0.4f * (1.0 - (i + 1) / std::max(length, 1.0f));

        Color trailColor = ColorAlpha(body.color, alpha);

        // Get the start and end points
        V p1 = body.trail[i] + offset;
        V p2 = body.trail[i+1] + offset;

        Vector3 startPoint = { (float)p1[0], (float)p1[1], (float)p1[2] };
        Vector3 endPoint = { (float)p2[0], (float)p2[1], (float)p2[2] };

        DrawCylinderEx(startPoint, endPoint, radiusStart, radiusEnd, 6, trailColor);
    }
}


void Renderer::drawCelestialBody(CelestialBody& body) {
    Vector3 pos = { (float)body.position[0], (float)body.position[1], (float)body.position[2] };

    // Drawing the celestial body
    DrawSphere(pos, body.radius, body.color);
}


void Renderer::display(Simulation* sim, UIComponent* ui) {
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(state->camera);

            // Draw each celestial body 
            for (auto& body : sim->bodies) {
                drawCelestialBody(*body);
                drawBodyTrail(*body);
                drawBodyLabel(body.get(), state->camera);
            }

            // Draw the explosion particles
            for (auto& particle : sim->particles) {
                drawParticle(*particle);
                drawParticleTrail(*particle);
            }

            // Draw the grid
            drawGravityGrid(sim->bodies);

        EndMode3D();

        // Display the fps, speed and pause state
        displaySpeed();

        // Draw the ui
        drawUI(ui);

        // Draw the help screen
        if (state->showHelp) drawHelp();

    EndDrawing();
}


void Renderer::displaySpeed() {
    // Draw the frame rate and time scale
    DrawText(TextFormat("%s fps", std::to_string((int)state->fps).c_str()), windowWidth - 200.0f, 0.0f, 30, WHITE);

    // Draw the simulation speed
    float speedDisplay = 0.0f;
    if (state->realTimeDelta > 0.0) {
        speedDisplay = state->simTimeDelta / state->realTimeDelta;
    }
    DrawText(TextFormat("%.2fxSpeed", speedDisplay), windowWidth - 200.0f, 30.0f, 30, WHITE);

    // Draw the pause state
    if (state->isPaused) {
        DrawText("Paused", windowWidth - 200.0f, 60.0f, 30, RED);
    } else {
        DrawText("Running", windowWidth - 200.0f, 60.0f, 30, GREEN);
    }
}


void Renderer::drawBodyLabel(CelestialBody* body, Camera3D& camera) {
    Vector3 worldPosition = { (float)body->position[0], (float)body->position[1], (float)body->position[2] };

    // Offset the y coordinate a little
    worldPosition.x -= body->radius;
    worldPosition.y += body->radius + 1.0f;

    Vector2 screenPosition = GetWorldToScreen(worldPosition, camera);

    DrawText(body->name.c_str(), screenPosition.x, screenPosition.y, 30, BLUE);
}


void Renderer::drawUI(UIComponent* ui) {
    if (!state->showMenu) {
        if (GuiButton({ 20, 20, 160, 30 }, "Open Editor")) 
            state->showMenu = true;
        return;
    }

    GuiGroupBox(ui->panelRect, "Create Celestial Body");

    // Name
    GuiLabel({ 30, 30, 100, 20 }, "Name");
    if (GuiTextBox({ 30, 50, 160, 30 }, ui->nameText, 64, ui->editNameMode))
        ui->editNameMode = !(ui->editNameMode);

    // Mass
    GuiLabel({ 30, 90, 100, 20 }, "Mass (in Earths)");
    if (GuiTextBox({ 30, 110, 160, 30 }, ui->massText, 64, ui->editMassMode))
        ui->editMassMode = !(ui->editMassMode);

    // Position
    GuiLabel({ 30, 150, 100, 20 }, "Position (X, Y, Z)");
    // X
    if (GuiTextBox({ 30, 180, 50, 30 }, ui->positionText[0], 64, ui->editPositionMode[0])) 
        ui->editPositionMode[0] = !(ui->editPositionMode[0]);
    // Y
    if (GuiTextBox({ 85, 180, 50, 30 }, ui->positionText[1], 64, ui->editPositionMode[1])) 
        ui->editPositionMode[1] = !(ui->editPositionMode[1]);
    // Z
    if (GuiTextBox({ 140,180, 50, 30 }, ui->positionText[2], 64, ui->editPositionMode[2])) 
        ui->editPositionMode[2] = !(ui->editPositionMode[2]);

    // Velocity
    GuiLabel({ 30, 220, 100, 20 }, "Velocity (m/s)");
    // X
    if (GuiTextBox({ 30, 240, 50, 30 }, ui->velocityText[0], 64, ui->editVelocityMode[0])) 
        ui->editVelocityMode[0] = !(ui->editVelocityMode[0]);
    // Y
    if (GuiTextBox({ 85, 240, 50, 30 }, ui->velocityText[1], 64, ui->editVelocityMode[1])) 
        ui->editVelocityMode[1] = !(ui->editVelocityMode[1]);
    // Z
    if (GuiTextBox({ 140, 240, 50, 30 }, ui->velocityText[2], 64, ui->editVelocityMode[2])) 
        ui->editVelocityMode[2] = !(ui->editVelocityMode[2]);


    // Radius
    GuiLabel({ 30, 280, 100, 20 }, "Radius (km)");
    if (GuiTextBox({ 140, 300, 50, 30 }, ui->radiusText, 64, ui->editRadiusMode))
        ui->editRadiusMode = !(ui->editRadiusMode);

    // Spawn Celestial Body
    if (GuiButton({ 30, 340, 160, 30 }, "Spawn Planet")) {
        ui->setNewBody();
    }

    // Edit the time scale
    GuiLabel({ 30, 380, 100, 20 }, "Time Scale (seconds)");
    if (GuiTextBox({ 140, 400, 50, 30 }, ui->timeScaleText, 64, ui->editTimeScaleMode)) {
        ui->editTimeScaleMode = !(ui->editTimeScaleMode);
        
        if (!(ui->editTimeScaleMode)) {
            ui->setTimeScale();
        }
    }

    // Pause simulation
    if (GuiButton({ 30, 440, 160, 30 }, "Toggle Simulation"))
        state->isPaused = !(state->isPaused);

    // Clear simulation
    if (GuiButton({ 30, 480, 160, 30 }, "Clear Simulation"))
        state->clear = true;

    // Close the editor
    if (GuiButton({ 30, 520, 160, 30 }, "Close Editor")) 
        state->showMenu = false;

    if (GuiButton({ 60, 560, 100, 20 }, "Help")) {
        state->showHelp = true;
        state->isPaused = true;
    }
}


void Renderer::drawHelp() {
    float w = windowWidth * 0.5f;
    float h = windowHeight * 0.5f;
    float x = (windowWidth - w) / 2.0f;
    float y = (windowHeight - h) / 2.0f;

    std::string helpContent = 
        "F: Toggle Fullscreen\n"
        "E: Pause Simulation\n"
        "C: Clear Simulation\n"
        "ESC: Unlock Camera\n"
        "Click: Enter Camera\n"
        "Right Click: Delete Planet";

    GuiGroupBox({x, y, w, h }, "Help");

    DrawText(helpContent.c_str(), x+20.0f, y+40.0f, 30, WHITE);

    if (GuiButton({ x+w-170, y+h-50, 160, 40 }, "Close")) {
        state->showHelp = false;
        state->isPaused = false;
    }
}