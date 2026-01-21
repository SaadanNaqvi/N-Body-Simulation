#ifndef GRAVITY_FORCE_H
#define GRAVITY_FORCE_H

#include "ForceModel.h"

class GravityForce: public ForceModel{
    private:
        double G = 6.673 * 10e-11;
        double softening2 = 1e14;

    public:
        Vector3 getForce(Particle* particle1, Particle* particle2) override;
        std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> getSystemForce(std::vector<Particle*>& systemParticles) override;

        void accumulateNetForces(std::vector<Particle*>& ps, std::vector<Vector3>& netF) override;
};


#endif