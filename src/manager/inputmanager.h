#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


#include "raylib.h"
#include "../simulation.h"
#include "../visuals/visuals.h"
#include "../types.h"

class InputManager {
public:
    InputManager(AppState* state) : 
        state(state)
    {
        SetExitKey(KEY_NULL);
    };

    void detectInput(Simulation* sim, UIComponent* ui);

    void processPicking(Simulation* sim);

private:
    AppState* state;
};

#endif