#include "visuals.h"
#include "physics/physics.h"
#include <string>


void UIComponent::setNewBody() {
    std::string name = nameText;
    double mass = std::stod(massText);
    float radius = std::stof(radiusText);
    V position = V(std::stof(positionText[0]), 
                std::stof(positionText[1]), 
                std::stof(positionText[2]));

    // Make a random colour
    Color randomColor = ColorFromHSV(GetRandomValue(0, 360), 0.8f, 0.9f);
    CelestialBody newBody = CelestialBody(
        name, randomColor, mass, radius, position
    );

    state->newBody = newBody;
}

void UIComponent::setTimeScale() {
    float timeScale = std::stof(timeScaleText);
    state->simTimeDelta = state->realTimeDelta * timeScale;

    // Reset the flag
    editTimeScaleMode = false;
}

bool UIComponent::isMouseOver() {
    return CheckCollisionPointRec(GetMousePosition(), panelRect);
}