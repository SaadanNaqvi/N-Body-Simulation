#include "Renderer.h"
#include <cmath>

static constexpr double G  = 6.67430e-11;
static constexpr double AU = 1.495978707e11;

Particle* makePlanetCircular(
    double r,          
    double mass,            
    double radius,          
    double centralMass,    
    double velJitter = 1.0    
){
    double x = r, y = 0.0, z = 0.0;

    double v = std::sqrt(G * centralMass / r) * velJitter;

    double vx = 0.0;
    double vy = v;
    double vz = 0.0;

    return new Particle(
        new Vector3(x, y, z),
        new Vector3(vx, vy, vz),
        new Vector3(0, 0, 0),
        mass,
        radius
    );
}

int main(){
    Renderer r(1500, 1080);

    Particle* sun = new Particle(
        new Vector3(0, 0, 0),
        new Vector3(0, 0, 0),
        new Vector3(0, 0, 0),
        1.9885e30,     
        6.9634e8    
    );

    System system;
    system.addParticle(sun);

    Particle* mercury = makePlanetCircular(0.387 * AU, 3.3011e23, 2.4397e6, sun->getMass());
    Particle* venus   = makePlanetCircular(0.723 * AU, 4.8675e24, 6.0518e6, sun->getMass());
    Particle* earth   = makePlanetCircular(1.000 * AU, 5.9720e24, 6.3710e6, sun->getMass());
    Particle* mars    = makePlanetCircular(1.524 * AU, 6.4171e23, 3.3895e6, sun->getMass());
    Particle* jupiter = makePlanetCircular(5.203 * AU, 1.8982e27, 6.9911e7, sun->getMass());
    Particle* saturn  = makePlanetCircular(9.537 * AU, 5.6834e26, 5.8232e7, sun->getMass());
    Particle* uranus  = makePlanetCircular(19.191* AU, 8.6810e25, 2.5362e7, sun->getMass());
    Particle* neptune = makePlanetCircular(30.070* AU, 1.0241e26, 2.4622e7, sun->getMass());

    system.addParticle(mercury);
    system.addParticle(venus);
    system.addParticle(earth);
    system.addParticle(mars);
    system.addParticle(jupiter);
    system.addParticle(saturn);
    system.addParticle(uranus);
    system.addParticle(neptune);

    {
        Vector3* ePos = earth->getPosition();
        Vector3* eVel = earth->getVelocity();

        const double rMoon = 384400000.0; 
        const double vMoon = std::sqrt(G * earth->getMass() / rMoon);

        Particle* moon = new Particle(
            new Vector3(ePos->getX() + rMoon, ePos->getY(), ePos->getZ()),
            new Vector3(eVel->getX(), eVel->getY() + vMoon, eVel->getZ()),
            new Vector3(0, 0, 0),
            7.34767309e22,
            1.7374e6
        );

        system.addParticle(moon);
    }

    r.run(system);
    return 0;
}
