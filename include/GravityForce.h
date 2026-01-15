#ifndef GRAVITY_FORCE_H
#define GRAVITY_FORCE_H

#include "ForceModel.h"

class GravityForce: public ForceModel{
    private:
        double G = 6.673 * 10e-11;

    public:
        double getForce(Particle* particle1, Particle* particle2) override;
        std::map<std::pair<Particle*, Particle*>, double> getSystemForce(std::vector<Particle*>) override;
};


#endif