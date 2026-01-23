#ifndef VELOCITY_VERLET_H
#define VELOCITY_VERLET_H

// used to integrate newtons equations of motion

#include "Integrator.h"

class System;

class VelocityVerlet: public Integrator{
    private:
        ForceWorker worker;
    public:
        void stepSimulation(double dt, System& system, std::vector<Vector3>& netF, double theta, double G, double eps) override;
};

#endif