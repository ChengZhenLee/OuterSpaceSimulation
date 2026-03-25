#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>


inline const std::string appTitle = "Outer Space Simulator";

// Visual constants
const int fps = 60;
const int windowWidth = 1600;
const int windowHeight = 900; 
const float limit = 200.0f;
const int gridSize = 50;
const double MIN_VISUAL_RADIUS = 0.1;
const float HALO_OPACITY = 0.3f;

// Simulation constants
const float friction = 0.9f;
const int substepCount = 100;

// Physical constants (in 10000km, Solar Masses and Seconds)
const double G = 39.47841760435743; 

// Conversion constants
const double EARTH_TO_SOLAR_MASS = 0.000003003489;
const double KM_TO_10K_KM = 1.0 / 10000.0;
const double MperS_TO_10K_KMperS = 1.0 / 10000000.0;

#endif