#include "visuals.h"
#include "../physics/physics.h"
#include <cstdlib>


void UIComponent::setNewBody() {
    std::string name = nameText;
    double mass = std::stod(massText);
    float radius = std::stod(radiusText);
    V position = V(std::stod(positionText[0]), 
                std::stod(positionText[1]), 
                std::stod(positionText[2]));

    // Make a random colour
    Color randomColor = ColorFromHSV(GetRandomValue(0, 360), 0.8f, 0.9f);
    CelestialBody newBody = CelestialBody(
        name, randomColor, mass, radius, position
    );

    state->newBody = newBody;
}


bool UIComponent::isMouseOver() {
    return CheckCollisionPointRec(GetMousePosition(), panelRect);
}