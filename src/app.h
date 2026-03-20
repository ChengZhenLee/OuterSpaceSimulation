#ifndef APP_H
#define APP_H

#include "raylib.h"
#include "../constants/constants.h"
#include "visuals/visuals.h"
#include "types.h"
#include "manager/inputmanager.h"


class App {
public:
    App() : 
        sim(Simulation()),
        ui(UIComponent(&state)),
        r(Renderer(&state)),
        im(InputManager(&state))
    { 
        InitWindow(windowWidth, windowHeight, "Outer Space Simulator");
        SetTargetFPS(fps);
        DisableCursor();
        SetExitKey(KEY_NULL);

        state.camera = getCamera();
        state.isPaused = false;
        state.cursorLocked = true;
        state.showMenu = false;
    }

    void run();

private:
    AppState state;

    Camera3D getCamera();

    Simulation sim;
    UIComponent ui;
    Renderer r;
    InputManager im;
};

#endif