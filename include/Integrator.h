#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "System.h"
#include <unordered_map>
#include "ForceWorker.h"
#include <vector>
#include <utility>

class System;

class Integrator{
    public:
        virtual void stepSimulation(double dt, System& system, std::vector<Vector3>& netF, double theta, double G, double eps)  = 0;
    
};


#endif