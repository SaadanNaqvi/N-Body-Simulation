#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "System.h"
#include <unordered_map>
#include <vector>
#include <utility>

class System;

class Integrator{
    public:
        virtual void stepSimulation(double dt, System& system)  = 0;
    
};


#endif