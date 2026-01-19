#include "System.h"

System::System(){}


System::System(std::vector<Particle*> particles){
    this->particles = particles;
}

void System::addParticle(Particle* particle){
    this->particles.push_back(particle*);
}

std::unordered_map<Particle*, Vector3> System::getForceOnEachParticle(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>>& systemForce){
    std::unordered_map<Particle*, Vector3> netForceOnParticles;
    for(auto& [particle, connections]: systemForce){
        Vector3 netForce(0.0, 0.0, 0.0);
        for(auto& [neighbour, force]: connections){
            netForce += force;
        }
        netForceOnParticles.push_back({particle, netForce});
    }

    return netForceOnParticles;
}

void System::update(){
    this->systemForce = gravityForce.getSystemForce(this->particles);
    this->netForceOnParticles = getForceOnEachParticle(this->systemForce);
    velocityVerlet.stepSimulation(1.0, gravityForce, systemForce, netForceOnParticles, particles);
}



vector<Particle*> System::getParticles(){
    return this->particles;
}

std::unordered_map<Particle*, Vector3> System::getNetForceOnParticles(){
    return thuis->netForceOnParticles;
}

std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> System::getSystemForce(){
    return this->systemForce;
}

GravityForce System::getGravityForce(){
    return this->gravityForce;
}





void System::setSystemForce(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> systemForce){
    this->systemForce = systemForce;
}