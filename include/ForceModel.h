#ifndef FORCE_MODEL_H
#define FORCE_MODEL_H

#include "Vector3.h"
#include "Particle.h"
#include <map>
#include <utility>

class ForceModel{
    private:
    public:
        virtual double getForce(Particle* particle1, Particle* particle2) = 0;
        virtual std::map<std::pair<Particle*, Particle*>, double> getSystemForce(std::vector<Particle*>) = 0;
};


#endif