#ifndef GRAVITY_FORCE_H
#define GRAVITY_FORCE_H

#include "ForceModel.h"

class GravityForce: public ForceModel{
    private:
        double G = 6.673 * 10e-11;

    public:
        Vector3 getForce(Particle* particle1, Particle* particle2) override;
        std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> getSystemForce(std::vector<Particle*>& systemParticles) override;
};


#endif