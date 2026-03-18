#include "ad.h"
#include <Eigen/Dense>
#include "constants.h"


template<typename T>
void calculatePotentialEnergy(
    std::vector<double> &masses, 
    std::vector<float> &radii,
    std::vector<Eigen::Matrix<T, 3, 1>> &positions,
    T &potentialEnergy
) 
{
    int length = masses.size(); 

    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < length; j++) {
            T r = (positions[i] - positions[j]).norm();

            if (r.value() > (radii[i] + radii[j])) {
                T Gm1m2(G * masses[i] * masses[j]);
                potentialEnergy = potentialEnergy - Gm1m2 / r;
            }
        }
    }
}


std::vector<Eigen::Matrix<float, 3, 1>> calculateForces(
    std::vector<double> masses, 
    std::vector<Eigen::Matrix<float, 3, 1>> positions,
    std::vector<float> radii
) {
    using A = ad::adjoint_t<float>;

    int length = masses.size();
    std::vector<Eigen::Matrix<float, 3, 1>> F(length);
    std::vector<Eigen::Matrix<A, 3, 1>> x(length);
    A y;

    // Register the inputs
    for (int i = 0; i < length; i++) {
        x[i][0].value() = positions[i][0];
        x[i][1].value() = positions[i][1];
        x[i][2].value() = positions[i][2];
        x[i][0].register_input();
        x[i][1].register_input();
        x[i][2].register_input();
    }

    // Build the tape
    y.value() = 0.0f;
    calculatePotentialEnergy<A>(masses, radii, x, y);

    // Seed
    A::tape::init_adjoints();
    y.adjoint(1);

    // Interpret
    A::tape::interpret();

    // Accumulate the derivatives
    // F = -dU/dx
    for (int i = 0; i < length; i++) {
        F[i][0] = -x[i][0].adjoint();
        F[i][1] = -x[i][1].adjoint();
        F[i][2] = -x[i][2].adjoint();
    }

    return F;
}


float getPotentialHeight(float x, float z,const std::vector<double>& masses, 
    const std::vector<Eigen::Matrix<float, 3, 1>>& positions) {

    
    float totalU = 0.0f;

    
    float G_VISUAL = 10.0f;

    
    Eigen::Vector3f gridPoint(x,0.0f, z);
    for (int i = 0; i < masses.size(); i++)
    {
        float dist = (gridPoint - positions[i]).norm();
        totalU -= (G_VISUAL * masses[i]) / (dist + 0.5f);
    }
    return totalU;
}