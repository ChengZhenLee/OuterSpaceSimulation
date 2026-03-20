#ifndef CAMERA_H
#define CAMERA_H


#include <Eigen/Dense>
#include "../physics/physics.h"
#include "../simulation.h"
#include "../constants/constants.h"


Camera3D getCamera();

void drawGravityGrid(std::vector<CelestialBody> &bodies);

class UIComponent {
public:
    // Constructor
    UIComponent(Simulation* sim) : 
        targetSim(sim) 
    {};

    bool isEditing = false;

    char nameText[64] = "newBody";
    bool editNameMode = false;

    char massText[64] = "100.0";
    bool editMassMode = false;

    char positionText[3][64] = { "0.0", "0.0", "0.0" };
    bool editPositionMode[3] = { false, false, false };

    char radiusText[64] = "0.25";
    bool editRadiusMode = false;

    void draw();

    bool isMouseOver();

private:
    Rectangle panelRect = { 20, 20, 200, 400 };
    Simulation* targetSim; 
};

class Renderer {
public:
    Renderer(Simulation* sim, UIComponent* ui) : 
        targetSim(sim),
        targetUI(ui)
    {
        InitWindow(windowWidth, windowHeight, "Outer Space Simulator");
        DisableCursor();
        SetTargetFPS(fps);
        setCamera();
    };

    bool cameraEnabled = true;
    Camera3D camera;

    void display();

    void drawBodyLabel(CelestialBody* body);

private:
    void setCamera();

    Simulation* targetSim;
    UIComponent* targetUI;
};

#endif