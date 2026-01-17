#include "Particle.h"

Particle::Particle(Vector* position, Vector3* velocity, Vector3* acceleration, double mass, double radius){
    this->position = position;
    this->velocity = velocity;   
    this->acceleration = acceleration;

    this->mass = mass;
    this->radius = radius;
}


double Particle::getMass(){
    return this->mass;
}

double getRadius(){
    return this->radius;
}

Vector3* getPosition(){
    return this->position;
}

Vector3* getVelocity(){
    return this->velocity;
}

Vector3* getAcceleration(){
    return this->acceleration;
}
