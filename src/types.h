#ifndef TYPES_H
#define TYPES_H

#include <optional>
#include "raylib.h"
#include "physics/physics.h"


struct AppState {
    Camera3D camera;
    
    bool isPaused;
    bool cursorLocked;
    bool showMenu;
    bool showHelp;
    bool clear;

    CelestialBody* hoveredBody = nullptr;
    CelestialBody* bodyToDelete = nullptr;
    std::optional<CelestialBody> newBody;

    float fps;
    float realTimeDelta;
    float simTimeDelta;
};

#endif