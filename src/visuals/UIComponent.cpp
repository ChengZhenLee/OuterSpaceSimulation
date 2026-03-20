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


    CelestialBody newBody = CelestialBody(
        name, GetColor(GetRandomValue(0, 0xFFFFFFFF)), mass, radius, position
    );

    state->newBody = newBody;
}


bool UIComponent::isMouseOver() {
    return CheckCollisionPointRec(GetMousePosition(), panelRect);
}