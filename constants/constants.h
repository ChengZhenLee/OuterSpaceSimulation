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

// Physical constants (in AU, Solar Masses and Years)
const double G = 39.47841760435743; 
const double SUN_MASS = 1.0;
const double EARTH_MASS = 0.00000300348959632;
const double EARTH_RADIUS = 0.000042635;
const double SUN_RADIUS = 0.00465047;

// Conversion constants
const double EARTH_TO_SOLAR_MASS = 0.000003003489;
const double KM_TO_AU = 1.0 / 149597870.7;

#endif