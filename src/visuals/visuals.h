#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "raygui.h"
#include <Eigen/Dense>
#include "../physics/physics.h"


class UIComponent {
    public:
        bool showMenu = true;
        bool editMode = false;
        bool isPaused = false;
        char massText[64] = "100.0";

        void draw();

    private:
        Rectangle panelRect = { 20, 20, 200, 300 };
};

Camera3D getCamera();

void drawGravityGrid(std::vector<CelestialBody> &bodies);

#endif