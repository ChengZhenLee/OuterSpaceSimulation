#include "constants.h"
#include "visuals.h"
#include <string>


double safeStringToDouble(const std::string &str) {
    // Check if the string is empty or contains only spaces
    if (str.empty() || str.find_first_not_of(' ') == std::string::npos) {
        return 0.0;
    }

    // Check if the string is a valid double
    try {
        return std::stod(str);
    } catch (...) {
        return 0.0;
    }
}


void UIComponent::setNewBody() {
    std::string name = nameText;
    double mass = safeStringToDouble(massText) * EARTH_TO_SOLAR_MASS;
    float radius = safeStringToDouble(radiusText) * KM_TO_10K_KM;
    V position = V(safeStringToDouble(positionText[0]), 
                safeStringToDouble(positionText[1]), 
                safeStringToDouble(positionText[2]));
    V velocity = V(safeStringToDouble(velocityText[0]), 
                safeStringToDouble(velocityText[1]), 
                safeStringToDouble(velocityText[2])) * MperS_TO_10K_KMperS;

    // Make a random colour
    Color randomColor = ColorFromHSV(GetRandomValue(0, 360), 0.8f, 0.9f);
    CelestialBody newBody = CelestialBody(
        name, randomColor, mass, radius, position, velocity
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