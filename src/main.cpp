#include "Renderer.h"
#include <cmath>
#include <random>

int main(){
    Renderer r(1000, 1000);

    Particle* earth = new Particle(
        new Vector3(0, 0, 0),
        new Vector3(0, 0, 0),
        new Vector3(0, 0, 0),
        5.972e24,
        6371e3
    );

    Particle* moon  = new Particle(
        new Vector3(384400000, 0, 0),
        new Vector3(0, 1022, 0),
        new Vector3(0, 0, 0),
        7.34767309e22,
        1.74e6
    );

    System system;
    system.addParticle(earth);
    system.addParticle(moon);

    int N = 200;
    double G = 6.67430e-11;

    double rMin = 1.0e8;  
    double rMax = 8.0e8; 

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> angDist(0.0, 2.0 * M_PI);
    std::uniform_real_distribution<double> radDist(rMin, rMax);
    std::uniform_real_distribution<double> zDist(-2.0e7, 2.0e7);  
    std::uniform_real_distribution<double> massDist(1e12, 1e16);       
    std::uniform_real_distribution<double> jitterDist(0.98, 1.02); 

    for(int i = 0; i < N; i++){
        double theta = angDist(rng);
        double rr = radDist(rng);
        double z = zDist(rng);

        double x = rr * std::cos(theta);
        double y = rr * std::sin(theta);

        double v = std::sqrt(G * earth->getMass() / rr) * jitterDist(rng);

        double vx = -v * std::sin(theta);
        double vy =  v * std::cos(theta);
        double vz = 0.0;

        double m = massDist(rng);

        double rad = 2.0e5;

        Particle* p = new Particle(
            new Vector3(x, y, z),
            new Vector3(vx, vy, vz),
            new Vector3(0, 0, 0),
            m,
            rad
        );

        system.addParticle(p);
    }

    r.run(system);
    return 0;
}
