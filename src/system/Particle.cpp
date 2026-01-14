#include "Particle.h"

Particle::Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, double mass, double radius){
    this->position = position;
    this->velocity = velocity;   
    this->acceleration = acceleration;

    this->mass = mass;
    this->radius = radius;
}