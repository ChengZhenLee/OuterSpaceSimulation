#include "ad.h"
#include <Eigen/Dense>
#include "constants.h"
#include "physics.h"


template<typename T>
//anziehungskraft der planeten
void calculatePotentialEnergy(
    std::vector<double> &masses, 
    std::vector<double> &radii,
    std::vector<Eigen::Matrix<T, 3, 1>> &positions,
    T &potentialEnergy
) 
{
    int length = masses.size(); 
    //acceleration can get too strong, so we soften the formula
    T softening(20.0);
    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < length; j++) {
            T r = (positions[i] - positions[j]).norm();
            //problem: if the planets get too near, the U grows really fast, which does tunnelling
            //we make a minimum distance so U doesnt get to infinity
            double minAllowedDist = radii[i] + radii[j];
            T minDistT(minAllowedDist);
            T effectiveR = (r < minDistT) ? minDistT : r;

            T Gm1m2(G * masses[i] * masses[j]);
            potentialEnergy = potentialEnergy - Gm1m2 / (effectiveR + softening);
        }
    }
}

//berechne F = -DeltaU
void calculateForces(
    std::vector<double> &masses, 
    std::vector<double> &radii,
    std::vector<V> &positions,
    std::vector<V> &F
) {
    using A = ad::adjoint_t<double>;

    int length = masses.size();
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
    y.adjoint() = 1;

    // Interpret
    A::tape::interpret();

    // Accumulate the derivatives
    // F = -dU/dx
    for (int i = 0; i < length; i++) {
        F[i][0] = x[i][0].adjoint();
        F[i][1] = x[i][1].adjoint();
        F[i][2] = x[i][2].adjoint();
    }

    A::tape::reset();
}

//neue position des planeten
void updatePosition(CelestialBody &body, V &acceleration, float timeDelta){
    body.position += body.velocity * timeDelta + 0.5 * acceleration * pow(timeDelta, 2);
};

//neue geschwindigkeit
void updateVelocity(CelestialBody &body, V &acceleration, float timeDelta){
    body.velocity = body.velocity + acceleration * timeDelta;
};


// Calculate forces and velocities when colliding
double resolveCollision(CelestialBody &a, CelestialBody &b, V relativePos, double distance){
    double overlap = (a.radius + b.radius) - distance;

    // Exit if not touching
    if (overlap <= 0) return 0.0;

    //direction of collision
    V collisionNormal = relativePos.normalized();

    // --Position Resolution--
    double totalMass = a.mass + b.mass;
    double ratioA = b.mass /totalMass;
    double ratioB = a.mass /totalMass;

    //nudge apart
    double extraMargin = 1.01;
    a.position -= collisionNormal * (overlap *ratioA)* extraMargin;
    b.position += collisionNormal * (overlap * ratioB)* extraMargin;

    // --Velocity Resolution--
    V relativeVelocity = a.velocity - b.velocity;
    double velocityAlongNormal = relativeVelocity.dot(collisionNormal);
    
    //if smaller zero both planets fly towards each other
    if(velocityAlongNormal < 0){
        float bounciness = 0.1f;
        //kick apart (impulse resolver)
        float j = -(1.0f + bounciness) * velocityAlongNormal;
        j /= (1.0f / a.mass + 1.0f / b.mass);

        V impulse = j * collisionNormal;

        // add to velocity the strength of kick in direction of collision
        a.velocity += impulse / a.mass;
        b.velocity -= impulse / b.mass; 

        // Tangential friction
        V tangentVelocity = relativeVelocity - (collisionNormal * velocityAlongNormal);
        float friction = friction;
        a.velocity -= tangentVelocity * friction * ratioA;
        b.velocity += tangentVelocity * friction * ratioB;
    }

    return std::abs(velocityAlongNormal);
}


// Shatter a body into fragments
std::vector<std::unique_ptr<CelestialBody>> shatterBody(CelestialBody body, double impactSpeed) {
    std::vector<std::unique_ptr<CelestialBody>> fragments;
    
    // Get a random number between 3 and 8
    int fragmentCount = rand() % 6 + 3;
    double fragMass = (body.mass / fragmentCount);

    // r = R / cuberoot(n)
    double fragRadius = (0.6 * body.radius / std::pow(fragmentCount, 1.0/3.0));

    // Just vaporize the body if its too small
    if (fragRadius < MIN_VISUAL_RADIUS) return {};

    // The blast power
    double blastPower = impactSpeed * 0.2f;

    for (int i = 0; i < fragmentCount; i++) {
        // 1. Generate a random unit vector for direction (numbers between -1 and 1)
        V randomDir = V(
            (rand() % 200 - 100) / 100.0,
            (rand() % 200 - 100) / 100.0,
            (rand() % 200 - 100) / 100.0
        ).normalized();

        // Spawn the fragments randomly within the original object's volume
        float spawnDist = body.radius * 1.2f;
        V position = body.position + randomDir * spawnDist;

        // Calculate the velocity of the object
        V velocity = body.velocity + randomDir * blastPower;

        CelestialBody fragment(
            TextFormat("%s %i", body.name.c_str(), i), body.color, fragMass, fragRadius, position, velocity
        );

        fragments.push_back(std::make_unique<CelestialBody>(fragment));
    }

    return fragments;
}


// Get the height (of a point on the grid) affected by mass
float getPotentialHeight(float x, float z, std::vector<double> &masses,  std::vector<V> &positions) {
    float totalU = 0.0f;
    
    Eigen::Vector3d gridPoint(x,0.0, z);
    for (int i = 0; i < masses.size(); i++)
    {
        float dist = (gridPoint - positions[i]).norm();
        totalU -= (G * masses[i]) / (dist + 0.5f);
    }
        
    return totalU;
}

