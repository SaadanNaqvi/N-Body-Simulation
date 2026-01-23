#ifndef SYSTEM_H
#define SYSTEM_H

#include "Particle.h"
#include "GravityForce.h"
#include <vector>
#include "VelocityVerlet.h"
#include "ForceWorker.h"
#include "Octree.h"
#include <random>
#include <cmath>
#include <chrono>
#include <iostream>

class VelocityVerlet;

class System{
    private:
        std::vector<Particle*> particles;
        std::vector<Vector3> netForcesVec;


        GravityForce gravityForce;
        VelocityVerlet* velocityVerlet;

        ForceWorker worker;
        std::vector<Vector3> acc;

        Octree octree;
    public:
        System();
        System(std::vector<Particle*> particles);
        void addParticle(Particle* particle);
        void randomSpawn();



        void update(double dt);

        std::vector<Particle*>& getParticles();
        GravityForce& getGravityForce();

        void computeAccelerationsBH(std::vector<Vector3>& outAcc,double theta, double G, double eps);

        ~System();

};  



#endif