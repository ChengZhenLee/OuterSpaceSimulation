#include "visuals.h"
#include "raylib.h"
#include <Eigen/Dense>
#include "physics/physics.h"


void drawGravityGrid(std::vector<CelestialBody> &bodies) {
    
    int length = bodies.size();
    std::vector<double> masses(length);
    std::vector<V> positions(length);

    for (int i = 0; i < length; i++) {
        masses[i] = bodies[i].mass;
        positions[i] = bodies[i].position;
    }

    float distanceBetweenGridpoints = 1.0f; // Grid spacing
    int size = 20;
    int gridCount = (size * 2) + 1; // total points in the grid

    //calculate y positons first(2D vector) 
    std::vector<std::vector<float>>heightGrid(gridCount, std::vector<float>(gridCount));
    
    for(int i = 0; i <gridCount; i++){
        // calculate x positions
        float x = (i - size) * distanceBetweenGridpoints;
        for(int j = 0; j < gridCount; j++){
            //calculate z positions
            float z = (j - size) * distanceBetweenGridpoints;
            //calculate height once per point
            heightGrid[i][j] = getPotentialHeight(x, z, masses, positions);
        }
    }
    for(int i = 0; i < gridCount; i++){
        float x = (i - size) * distanceBetweenGridpoints;
        for(int j = 0; j < gridCount; j++){
            
            float z = (j - size) * distanceBetweenGridpoints;
            float y = heightGrid[i][j];
            Vector3 p1 ={x, y ,z};
            
            //connect to right neighbor
            if(i < gridCount - 1){
                float yRight = heightGrid[i+1][j];
                DrawLine3D(p1, {x + distanceBetweenGridpoints, yRight, z}, DARKGRAY);
            }

            //connect to front neighbor
            if(j < gridCount - 1){
                float yFront = heightGrid[i][j+1];
                DrawLine3D(p1, {x, yFront, z + distanceBetweenGridpoints}, DARKGRAY);
            }
        }
    }
}