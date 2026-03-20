#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "visuals.h"
#include "../physics/physics.h"


void UIComponent::draw() {
    if (!isEditing) {
        // Button to open editor
        if (GuiButton({ 20, 20, 160, 30 }, "Open Editor")) 
            isEditing = true;

    } else {

        // Draw a background panel
        GuiGroupBox(panelRect, "Create Celestial Body");

        // Name
        GuiLabel({ 30, 50, 100 , 20}, "Name");
        if (GuiTextBox((Rectangle){ 30, 70, 160, 30 }, nameText, 64, editNameMode))
            editNameMode = !editNameMode;

        // Mass
        GuiLabel({ 30, 100, 100 , 20}, "Mass");
        if (GuiTextBox((Rectangle){ 30, 120, 160, 30 }, massText, 64, editMassMode))
            editMassMode = !editMassMode;

        // Position
        GuiLabel({ 30, 160, 100, 20 }, "Position (X, Y, Z)");
        // X
        if (GuiTextBox({ 30, 180, 50, 30 }, positionText[0], 64, editPositionMode[0])) 
            editPositionMode[0] = !editPositionMode[0];
        // Y
        if (GuiTextBox({ 85, 180, 50, 30 }, positionText[1], 64, editPositionMode[1])) 
            editPositionMode[1] = !editPositionMode[1];
        // Z
        if (GuiTextBox({ 140, 180, 50, 30 }, positionText[2], 64, editPositionMode[2])) 
            editPositionMode[2] = !editPositionMode[2];

        // Radius
        GuiLabel({ 30, 220, 100, 20 }, "Radius");
        if (GuiTextBox({ 140, 240, 50, 30 }, radius, 64, editRadiusMode))
            editRadiusMode = !editRadiusMode;

        // Button
        if (GuiButton({ 30, 280, 160, 30 }, "Spawn Planet")) {
            try {
                double m = std::stof(massText);
                float px = std::stof(positionText[0]);
                float py = std::stof(positionText[1]);
                float pz = std::stof(positionText[2]);
                
                // Create and add a new body to the simulation
                CelestialBody newBody = CelestialBody(
                    "test", WHITE, m, 1.0, { px, py, pz }
                );
                targetSim->addBody(newBody);

            } catch (...) {
                TraceLog(LOG_WARNING, "Invalid input");
            }
        }

        // Pause simulation
        if (GuiButton({ 30, 320, 160, 30 }, "Toggle Simulation"))
            targetSim->isPaused = !(targetSim->isPaused);

        // Close the editor
        if (GuiButton({ 30, 360, 160, 30 }, "Close Editor")) 
            isEditing = false;
    }
}


bool UIComponent::isMouseOver() {
    return CheckCollisionPointRec(GetMousePosition(), panelRect);
}