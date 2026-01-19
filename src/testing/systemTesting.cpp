#include <iostream>
#include "System.h"
using namespace std;

int main(){
    
    Particle* earth = new Particle(new Vector3(0, 0, 0), new Vector3(0, 0, 0), new Vector3(0, 0, 0), 5.972e24, 6371e3);
    Particle* moon  = new Particle(new Vector3(384400000, 0, 0), new Vector3(0, 1022, 0), new Vector3(0, 0, 0), 7.34767309e22,1.74e6);
    System system;

    system.addParticle(earth);
    system.addParticle(moon);
    
    for (int i = 0; i < 10; i++) {
        system.update(); 

        // Print positions
        std::cout << "Step " << i << ":\n";
        for (auto& p : system.getParticles()) {
            Vector3* pos = p->getPosition();
            std::cout << "Particle at (" << pos->getX() << ", " << pos->getY() << ", " << pos->getZ() << ")\n";
        }
        std::cout << "---------------------\n";
    }
    
}