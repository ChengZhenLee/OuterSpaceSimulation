#include "ad.h"
#include <Eigen/Dense>
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
    T softening(1);
    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < length; j++) {
            T r = (positions[i] - positions[j]).norm();
            //problem: if the planets get too near, the U grows really fast, which does tunnelling
            //we make a minimum distance so U doesnt get to infinity
            double minAllowedDist = radii[i] + radii[j];
            T minDistT(minAllowedDist);
            T effectiveR = (r < minDistT) ? minDistT : r;
            //falls zwei planeten sich berühren keine gravity mehr
            //if (r.value() > (radii[i] + radii[j])) {
                T Gm1m2(G * masses[i] * masses[j]);
                potentialEnergy = potentialEnergy - Gm1m2 / (effectiveR + softening);
            //}
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


//-----------------COLLISION THING START---------------------

void resolveCollision(CelestialBody &a, CelestialBody &b, V relativePos, double distance){
    double overlap = (a.radius + b.radius) - distance;

    // Exit if not touching
    if (overlap <= 0) return;

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
}


void handleCollisions(std::vector<CelestialBody> &bodies){
    for(int i = 0; i < bodies.size(); i++){
        for(int j = i+1; j < bodies.size(); j++){
            //vector that points from center of Planet A to center of planet B
            V relativePos = bodies[j].position - bodies[i].position;

            double distance = relativePos.norm();

            double minDistance = bodies[i].radius + bodies[j].radius;

            if(distance < minDistance){
                //if theres a collision
                resolveCollision(bodies[i], bodies[j], relativePos, distance);
            }

        }
    }
}

//--------------------COLLISION THING END-----------------------------------------


void updateBodies(std::vector<CelestialBody> &bodies, float timeDelta) {
    int length = bodies.size();
    std::vector<V> F(length);
    std::vector<double> masses(length);
    std::vector<double> radii(length);
    std::vector<V> positions(length);
    for (int i = 0; i < length; i++) {
        masses[i] = bodies[i].mass;
        radii[i] = bodies[i].radius;
        positions[i] = bodies[i].position;
    }

    calculateForces(masses, radii, positions, F);

    for (int i = 0; i < length; i++) {
        V acceleration = F[i] / masses[i];
        bodies[i].velocity += acceleration * timeDelta;   // Update speed based on current gravity
        bodies[i].position += bodies[i].velocity * timeDelta; // Move using the NEW speed
        
    }
    handleCollisions(bodies);
    
}


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

