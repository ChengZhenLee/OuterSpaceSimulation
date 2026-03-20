#include "raylib.h"
#include "visuals/visuals.h"
#include "manager/inputmanager.h"
#include "simulation.h"


int main(void) {
    // Create the simulation
    Simulation sim = Simulation({});

    // Create the UI component
    UIComponent ui = UIComponent(&sim);

    // Create the visual component
    Renderer r = Renderer(&sim, &ui);

    // Create the input manager
    InputManager im = InputManager(&sim, &ui, &r);

    while (!WindowShouldClose()) {
        // Detect any inputs from users
        im.detectInput();

        sim.update();

        r.display();
    }

    CloseWindow();

    return 0;
}