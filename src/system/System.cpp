#include "System.h"

System::System(){}


System::System(std::vector<Particle*> particles){
    this->particles = particles;
    velocityVerlet = new VelocityVerlet();
}

void System::addParticle(Particle* particle){
    this->particles.push_back(particle);
    velocityVerlet = new VelocityVerlet();
}

std::unordered_map<Particle*, Vector3> System::getForceOnEachParticle(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>>& systemForce){
    std::unordered_map<Particle*, Vector3> netForceOnParticles;
    for(auto& [particle, connections]: systemForce){
        Vector3 netForce;
        for(auto& [neighbour, force]: connections){
            netForce += force;
        }
        netForceOnParticles[particle] = netForce;
    }

    return netForceOnParticles;
}

void System::update(double dt){
    this->systemForce = gravityForce.getSystemForce(this->particles);
    this->netForceOnParticles = getForceOnEachParticle(this->systemForce);
    velocityVerlet->stepSimulation(dt, *this);
}



std::vector<Particle*>& System::getParticles(){
    return this->particles;
}

std::unordered_map<Particle*, Vector3>& System::getNetForceOnParticles(){
    return this->netForceOnParticles;
}

std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> System::getSystemForce(){
    return this->systemForce;
}

GravityForce& System::getGravityForce(){
    return this->gravityForce;
}





void System::setSystemForce(std::unordered_map<Particle*, std::vector<std::pair<Particle*, Vector3>>> systemForce){
    this->systemForce = systemForce;
}

void System::randomSpawn(){
    for (Particle* p : particles) delete p;


    particles.clear();
    systemForce.clear();
    netForceOnParticles.clear();

    static std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<double> angDist(0.0, 2.0 * M_PI);
    std::uniform_real_distribution<double> zDist(-5e9, 5e9);
    std::uniform_real_distribution<double> jitter(0.97, 1.03);
    std::uniform_real_distribution<double> massDist(1e20, 1e23);

    Particle* sun = new Particle(
        new Vector3(0, 0, 0),
        new Vector3(0, 0, 0),
        new Vector3(0, 0, 0),
        1.989e30,       
        6.96e8          
    );
    particles.push_back(sun);


    const int n = 200;
    const double G = 6.67430e-11;
    const double rMin = 5.0e10;
    const double rMax = 4.0e11;
    std::uniform_real_distribution<double> radDist(rMin, rMax);

    for (int i = 0; i < n; i++) {
        double r = radDist(rng);
        double theta = angDist(rng);
        double z = zDist(rng);

        double x = r *std::cos(theta);
        double y = r *std::sin(theta);

        double v = std::sqrt(G * sun->getMass() / r) * jitter(rng);

        double vx = -v * std::sin(theta);
        double vy =  v * std::cos(theta);
        double vz =  0.0;

        double m = massDist(rng);
        double radius = 1e7;

        Particle* p = new Particle(
            new Vector3(x, y, z),
            new Vector3(vx, vy, vz),
            new Vector3(0, 0, 0),
            m,
            radius
        );

        particles.push_back(p);
    }

    {
        Vector3 totalP(0, 0, 0); 

        for (Particle* p : particles) {
            if (p == sun) continue;

            Vector3* v = p->getVelocity();
            totalP += Vector3(v->getX(), v->getY(), v->getZ()) * p->getMass();
        }

        Vector3 vSun = totalP * (-1.0 / sun->getMass());
        sun->setVelocity(new Vector3(vSun.getX(), vSun.getY(), vSun.getZ()));
    }

    

    delete velocityVerlet;
    velocityVerlet = new VelocityVerlet();

}

System::~System() {
    delete velocityVerlet;
}