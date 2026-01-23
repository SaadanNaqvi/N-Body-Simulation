#include "VelocityVerlet.h"

void VelocityVerlet::stepSimulation(double dt, System& system, std::vector<Vector3>& netF, double theta, double G, double eps){
    auto& particles = system.getParticles();
    int n = (int)particles.size();
    if (n == 0) return;

    std::vector<Vector3> aOld(n), aNew(n);

    worker.parallelFor(0, n, [&](int i){
        aOld[i] = netF[i] / particles[i]->getMass();
        Particle* p = particles[i];
        Vector3 x = p->getPosition();
        Vector3 v = p->getVelocity();
        p->setPosition(x + v*dt + aOld[i]*(0.5*dt*dt));
    });

    std::vector<Vector3> netFNew(n, Vector3(0,0,0));
    system.computeAccelerationsBH(aNew, theta, G, eps);

    worker.parallelFor(0, n, [&](int i){
        Particle* p = particles[i];
        Vector3 v = p->getVelocity();
        Vector3 vNew = v + (aOld[i] + aNew[i])*(0.5*dt);
        p->setVelocity(vNew);
    });
}
