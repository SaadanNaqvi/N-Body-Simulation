#ifndef SYSTEM_H
#define SYSTEM_H

#include "Particle.h"
#include "GravityForce.h"
#include <vector>
#include "VelocityVerlet.h"
#include <random>
#include <cmath>

class VelocityVerlet;

class System{
    private:
        std::vector<Particle*> particles;
        std::unordered_map<Particle*, Vector3> netForceOnParticles;
        std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> systemForce;


        GravityForce gravityForce;
        VelocityVerlet* velocityVerlet;
        
    public:
        System();
        System(std::vector<Particle*> particles);
        void addParticle(Particle* particle);
        void randomSpawn();



        void update(double dt);
        std::unordered_map<Particle*, Vector3> getForceOnEachParticle(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>>& systemForce);

        std::vector<Particle*>& getParticles();
        std::unordered_map<Particle*, Vector3>& getNetForceOnParticles();
        std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> getSystemForce();
        GravityForce& getGravityForce();


        void setSystemForce(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> systemForce);

        ~System();

};  



#endif