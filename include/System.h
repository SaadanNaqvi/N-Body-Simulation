#ifndef SYSTEM_H
#define SYSTEM_H

#include "Particle.h"
#include "GravityForce.h"
#include "VelocityVerlet.h"

class System{
    private:
        std::vector<Particle*> particles;
        std::unordered_map<Particle*, Vector3> netForceOnParticles;
        std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> systemForce;


        GravityForce gravityForce;
        VelocityVerlet velocityVerlet;
        
    public:
        System();
        System(std::vector<Particle*> particles);
        
        void addParticle(Particle* particle);



        void update()
        std::vector<std::pair<Particle*, Vector3>> getForceOnEachParticle(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>>& systemForce);

        vector<Particle*>& getParticles();
        std::unordered_map<Particle*, Vector3>& getNetForceOnParticles();
        std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> getSystemForce();
        GravityForce& getGravityForce();


        void setSystemForce(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> systemForce);

};  



#endif