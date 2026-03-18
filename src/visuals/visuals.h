#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include <Eigen/Dense>


Camera3D getCamera();

void DrawGravityGrid(const std::vector<double> &masses, const std::vector<Eigen::Matrix<float, 3, 1>> &positions);

#endif