#ifndef CAMERA_H
#define CAMERA_H


#include <Eigen/Dense>
#include "../physics/physics.h"
#include "../simulation.h"
#include "../types.h"

Camera3D getCamera();

void drawGravityGrid(std::vector<CelestialBody> &bodies);

class UIComponent {
public:
    // Constructor
    UIComponent(AppState* state) : 
        state(state) 
    {};

    Rectangle panelRect = { 20, 20, 200, 400 };

    char nameText[64] = "newBody";
    bool editNameMode = false;

    char massText[64] = "100.0";
    bool editMassMode = false;

    char positionText[3][64] = { "0.0", "0.0", "0.0" };
    bool editPositionMode[3] = { false, false, false };

    char radiusText[64] = "0.25";
    bool editRadiusMode = false;

    void setNewBody();

    bool isMouseOver();

private:
    AppState* state;
};

class Renderer {
public:
    Renderer(AppState* state) : 
        state(state)
    {};

    bool cameraEnabled = true;

    void display(Simulation* sim, UIComponent* ui);

    void drawBodyLabel(CelestialBody* body, Camera3D camera);

    void drawUI(UIComponent* ui);

private:
    AppState* state;
};

#endif