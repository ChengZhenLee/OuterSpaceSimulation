#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


#include "raylib.h"
#include "../simulation.h"
#include "../visuals/visuals.h"

class InputManager {
public:
    InputManager(Simulation* sim, UIComponent* ui, Renderer* r) : 
        targetSim(sim),
        targetUI(ui),
        targetRenderer(r)
    {
        SetExitKey(KEY_NULL);
    };

    void detectInput();

    void processPicking();

private:
    Simulation* targetSim;
    UIComponent* targetUI;
    Renderer* targetRenderer;
};

#endif