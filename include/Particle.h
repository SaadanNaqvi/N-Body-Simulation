#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

class Particle{
    private:
        double mass;
        double radius;
    protected:
        Vector3 position;
        Vector3 velocity;   
        Vector3 acceleration;
    public:
        Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, double mass, double radius);

        // Getters:
        double getMass();
        double getRadius();
        Vector3 getPosition();
        Vector3 getVelocity();
        Vector3 getAcceleration();

        // Setters:
        void setPosition(Vector3 position);
        void setVelocity(Vector3 velocity);
        void setAcceleration(Vector3 acceleration);


};


#endif