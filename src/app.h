#ifndef APP_H
#define APP_H

#include "raylib.h"
#include "constants.h"
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
        InitWindow(windowWidth, windowHeight, appTitle.c_str());
        SetTargetFPS(fps);
        DisableCursor();
        SetExitKey(KEY_NULL);

        state.camera = getCamera();
        state.isPaused = false;
        state.cursorLocked = true;
        state.showMenu = false;
        state.showHelp = false;
        state.clear = false;

        state.realTimeDelta = 1.0f / fps;
        state.simTimeDelta = state.realTimeDelta;
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