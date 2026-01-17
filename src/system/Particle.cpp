#include "Particle.h"

Particle::Particle(Vector3* position, Vector3* velocity, Vector3* acceleration, double mass, double radius){
    this->position = position;
    this->velocity = velocity;   
    this->acceleration = acceleration;

    this->mass = mass;
    this->radius = radius;
}


double Particle::getMass(){
    return mass;
}

double Particle::getRadius(){
    return radius;
}

Vector3* Particle::getPosition(){
    return position;
}

Vector3* Particle::getVelocity(){
    return velocity;
}

Vector3* Particle::getAcceleration(){
    return acceleration;
}
