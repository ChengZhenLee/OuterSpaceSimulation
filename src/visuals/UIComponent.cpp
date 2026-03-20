#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "visuals.h"


void UIComponent::draw() {
    if (!showMenu) return;

    // Draw a background panel
    GuiGroupBox(panelRect, "Create Celestial Body");

    // Mass
    GuiLabel({ 30, 50, 100 , 20}, "Mass");
    if (GuiTextBox((Rectangle){ 30, 70, 160, 30 }, massText, 64, editMassMode))
        editMassMode = !editMassMode;   // Toggle focus when clicked

    // Position
    GuiLabel({ 30, 110, 100, 20 }, "Position (X, Y, Z)");
    // X
    if (GuiTextBox({ 30, 130, 50, 30 }, positionText[0], 64, editPositionMode[0])) 
        editPositionMode[0] = !editPositionMode[0];
    // Y
    if (GuiTextBox({ 85, 130, 50, 30 }, positionText[1], 64, editPositionMode[1])) 
        editPositionMode[1] = !editPositionMode[1];
    // Z
    if (GuiTextBox({ 140, 130, 50, 30 }, positionText[2], 64, editPositionMode[2])) 
        editPositionMode[2] = !editPositionMode[2];

    // Button
    if (GuiButton({ 30, 200, 160, 30 }, "Spawn Planet")) {
        try {
            float m = std::stof(massText);
            float px = std::stof(positionText[0]);
            float py = std::stof(positionText[1]);
            float pz = std::stof(positionText[2]);

            // targetSim->addBody();

            TraceLog(LOG_WARNING, "Planet spawned");
        } catch (...) {
            TraceLog(LOG_WARNING, "Invalid input");
        }
    }
}