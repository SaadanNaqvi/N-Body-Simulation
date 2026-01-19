#ifndef FORCE_MODEL_H
#define FORCE_MODEL_H

#include "Vector3.h"
#include "Particle.h"
#include <unordered_map>
#include <vector>
#include <utility>

class ForceModel{
    private:
    public:
        virtual Vector3 getForce(Particle* particle1, Particle* particle2) = 0;
        virtual std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> getSystemForce(std::vector<Particle*>& systemParticles) = 0;
};


#endif