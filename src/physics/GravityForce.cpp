#include "GravityForce.h"

Vector3 GravityForce::getForce(Particle* particle1, Particle* particle2){
    double dx = particle2->getPosition().getX() - particle1->getPosition().getX();
    double dy = particle2->getPosition().getY() - particle1->getPosition().getY();
    double dz = particle2->getPosition().getZ() - particle1->getPosition().getZ();

    double distance = (dx * dx) + (dy * dy) + (dz * dz);
    double r = sqrt(distance);
    double force = (G * particle1->getMass() * particle2->getMass()) / distance;

    double fx = force * dx / r;
    double fy = force * dy / r;
    double fz = force * dz / r;

    Vector3 forces(fx,fy,fz);

    return forces;
}

std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> GravityForce::getSystemForce(std::vector<Particle*>& systemParticles){
    int n = systemParticles.size();
    std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> systemForce;

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            Vector3 particleForce = getForce(systemParticles[i], systemParticles[j]);
            systemForce[systemParticles[i]].push_back({systemParticles[j], particleForce});
            systemForce[systemParticles[j]].push_back({systemParticles[i], particleForce * -1.0});
        }
    }

    return systemForce;
}

void GravityForce::accumulateNetForces(std::vector<Particle*>& ps, std::vector<Vector3>& netF){
    int n = ps.size();

    for(int i = 0; i < n; i++){
        auto pi = ps[i]->getPosition();
        double mi = ps[i]->getMass();

        for (int j = i+1; j < n; j++){
            auto pj = ps[j]->getPosition();
            double mj = ps[j]->getMass();

            double dx = pj.getX() - pi.getX();
            double dy = pj.getY() - pi.getY();
            double dz = pj.getZ() - pi.getZ();

            double r2 = dx*dx + dy*dy + dz*dz + softening2;
            double invR = 1.0 / std::sqrt(r2);
            double invR3 = invR * invR * invR;

            double s = G * mi * mj * invR3;

            Vector3 fij(dx * s, dy * s, dz * s);

            netF[i] += fij;
            netF[j] += fij * -1.0;
        }
    }

}