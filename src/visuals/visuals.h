#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include "../physics/physics.h"
#include "../simulation.h"


class UIComponent {
public:
    // Constructor
    UIComponent(Simulation* sim) : 
        targetSim(sim) 
    {};

    bool showMenu = true;
    bool editMassMode = false;
    bool editPositionMode[3] = { false, false, false };
    bool isPaused = false;

    char massText[64] = "100.0";
    char positionText[3][64] = { "0.0", "0.0", "0.0" };

    void draw();

private:
    Rectangle panelRect = { 20, 20, 200, 300 };
    Simulation* targetSim; 
};

Camera3D getCamera();

void drawGravityGrid(std::vector<CelestialBody> &bodies);

#endif