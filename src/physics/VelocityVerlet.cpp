#include "VelocityVerlet.h"

void VelocityVerlet::stepSimulation(double dt, System& system) {
    
    auto& particles = system.getParticles();            
    auto& netForceOnParticles = system.getNetForceOnParticles();       
    auto& gravityForce = system.getGravityForce();             

    for(auto& particle : particles){
        Vector3 acceleration = netForceOnParticles[particle] / particle->getMass();
        particle->setPosition(
            Vector3(
                (particle->getPosition()) + (particle->getVelocity()) * dt + acceleration * (0.5 * dt * dt)
            )
        );
    }

    auto systemForce = gravityForce.getSystemForce(particles);
    system.setSystemForce(systemForce);

    auto newNetForceOnParticles = system.getForceOnEachParticle(systemForce);



    for(auto& particle : particles){
        Vector3 aOld = netForceOnParticles[particle] / particle->getMass();
        Vector3 aNew = newNetForceOnParticles[particle] / particle->getMass();
        particle->setVelocity(
            Vector3(
                (particle->getVelocity()) + (aOld + aNew) * (0.5 * dt)
            )
        );
    }

    netForceOnParticles = newNetForceOnParticles;
}