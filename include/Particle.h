#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

class Particle{
    private:
        Vector3 position;
        Vector3 velocity;   
        Vector3 acceleration;

        double mass;
        double radius;
    public:
};


#endif