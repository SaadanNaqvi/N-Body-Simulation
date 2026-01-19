#ifndef VELOCITY_VERLET_H
#define VELOCITY_VERLET_H

// used to integrate newtons equations of motion

#include "Integrator.h"

class System;

class VelocityVerlet: public Integrator{
    private:
    public:
        void stepSimulation(double dt, System& system) override;
};

#endif