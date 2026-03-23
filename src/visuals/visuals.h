#ifndef CAMERA_H
#define CAMERA_H


#include <Eigen/Dense>
#include "physics/physics.h"
#include "physics/simulation.h"
#include "types.h"


class UIComponent {
public:
    // Constructor
    UIComponent(AppState* state) : 
        state(state) 
    {};

    Rectangle panelRect = { 20, 20, 200, 520 };

    char nameText[64] = "newBody";
    bool editNameMode = false;

    char massText[64] = "100.0";
    bool editMassMode = false;

    char positionText[3][64] = { "0.0", "0.0", "0.0" };
    bool editPositionMode[3] = { false, false, false };

    char radiusText[64] = "0.25";
    bool editRadiusMode = false;

    char timeScaleText[64] = "1.0";
    bool editTimeScaleMode = false;

    void setNewBody();

    void setTimeScale();

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

    void drawGravityGrid(std::vector<CelestialBody>& bodies);

    void drawCelestialBody(CelestialBody& body);

    void display(Simulation* sim, UIComponent* ui);

    void drawBodyLabel(CelestialBody* body, Camera3D& camera);

    void drawUI(UIComponent* ui);

private:
    AppState* state;
};

#endif