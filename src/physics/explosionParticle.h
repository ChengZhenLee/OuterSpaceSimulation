#ifndef EXPLOSIONPARTICLE_H
#define EXPLOSIONPARTICLE_H


#include <deque>
#include "raylib.h"
#include "constants.h"


class ExplosionParticle {
public:
    V position;
    V velocity;
    Color color;
    float life;
    float radius;

    std::deque<V> trail;

    void updateTrail() {
        trail.push_front(position);
        if (trail.size() > MAXTRAILLENGTH) trail.pop_back();
    }
};

#endif