#include "GravityForce.h"

double GravityForce::getForce(Particle* particle1, Particle* particle2){
    double dx = particle2->getPosition()->getX() - particle1->getPosition()->getX();
    double dy = particle2->getPosition()->getY() - particle1->getPosition()->getY();
    double dz = particle2->getPosition()->getZ() - particle1->getPosition()->getZ();

    double distance = (dx * dx) + (dy * dy) + (dz * dz);
    double force = (G * particle1->getMass() * particle2->getMass()) / distance;

    return force;
}

std::map<std::pair<Particle*, Particle*>, double> GravityForce::getSystemForce(std::vector<Particle*>){
    return {};
}