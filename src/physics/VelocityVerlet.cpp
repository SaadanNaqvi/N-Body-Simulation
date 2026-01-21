#include "VelocityVerlet.h"

void VelocityVerlet::stepSimulation(double dt, System& system, std::vector<Vector3>& netF) {
    auto& particles = system.getParticles();
    const int n = (int)particles.size();
    if (n == 0) return;

    std::vector<Vector3> aOld(n);
    for (int i = 0; i < n; i++){
        aOld[i] = netF[i] / particles[i]->getMass();
    }

    for (int i = 0; i < n; i++){
        Particle* p = particles[i];
        Vector3 x = p->getPosition();
        Vector3 v = p->getVelocity();

        Vector3 xNew = x + v * dt + aOld[i] * (0.5 * dt * dt);
        p->setPosition(xNew);
    }

    std::vector<Vector3> netFNew(n, Vector3(0,0,0));
    system.getGravityForce().accumulateNetForces(particles, netFNew);

    for (int i = 0; i < n; i++){
        Particle* p = particles[i];
        Vector3 v = p->getVelocity();
        Vector3 aNew = netFNew[i] / p->getMass();

        Vector3 vNew = v + (aOld[i] + aNew) * (0.5 * dt);
        p->setVelocity(vNew);
    }
}
