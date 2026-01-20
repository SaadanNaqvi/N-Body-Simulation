#include "Renderer.h"

int main(){
    Renderer r(1000, 1000);
    
    Particle* earth = new Particle(new Vector3(0, 0, 0), new Vector3(0, 0, 0), new Vector3(0, 0, 0), 5.972e24, 6371e3);
    Particle* moon  = new Particle(new Vector3(384400000, 0, 0), new Vector3(0, 1022, 0), new Vector3(0, 0, 0), 7.34767309e22,1.74e6);
    Particle* me  = new Particle(new Vector3(3219392103, 2131313, -141313), new Vector3(23231, 1022, -131333), new Vector3(33,1, 0), 7.34767309e22,1.74e9);
    System system;
    system.addParticle(earth);
    system.addParticle(moon);
    system.addParticle(me);
    
    


    r.run(system);
    return 0;
}