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

void System::randomSpawn() {
    for (Particle* p : particles) delete p;
    particles.clear();
    systemForce.clear();
    netForceOnParticles.clear();

    static std::mt19937 rng(std::random_device{}());

    const int N = 250;      
    const double region = 1.5e11; 
    const double maxSpeed = 15000.0; 
    const double mMin = 1e22;       
    const double mMax = 5e26;        
    const double radiusBase = 5e6;    
    const bool makeBinary = true;      

    std::uniform_real_distribution<double> posDist(-region, region);
    std::uniform_real_distribution<double> speedDist(-maxSpeed, maxSpeed);
    std::uniform_real_distribution<double> massDist(mMin, mMax);

    particles.reserve(N);

    for (int i = 0; i < N; i++) {
        double x = posDist(rng);
        double y = posDist(rng);
        double z = posDist(rng);

        double vx = speedDist(rng);
        double vy = speedDist(rng);
        double vz = speedDist(rng);

        double m = massDist(rng);

        double rVis = radiusBase * std::cbrt(m / 1e24);

        particles.push_back(new Particle(
            Vector3(x, y, z),
            Vector3(vx, vy, vz),
            Vector3(0, 0, 0),
            m,
            rVis
        ));
    }

    {
        Vector3 comPos(0,0,0);
        Vector3 comVel(0,0,0);
        double totalM = 0.0;

        for (Particle* p : particles) {
            double m = p->getMass();
            totalM += m;

            Vector3 x = p->getPosition();
            Vector3 v = p->getVelocity();

            comPos += Vector3(x.getX(), x.getY(), x.getZ()) * m;
            comVel += Vector3(v.getX(), v.getY(), v.getZ()) * m;
        }

        comPos = comPos * (1.0 / totalM);
        comVel = comVel * (1.0 / totalM);

        for (Particle* p : particles) {
            Vector3 x = p->getPosition();
            Vector3 v = p->getVelocity();

            Vector3 newX = Vector3(x.getX(), x.getY(), x.getZ()) - comPos;
            Vector3 newV = Vector3(v.getX(), v.getY(), v.getZ()) - comVel;

            p->setPosition(Vector3(newX.getX(), newX.getY(), newX.getZ()));
            p->setVelocity(Vector3(newV.getX(), newV.getY(), newV.getZ()));
        }
    }

    // --- Inject a guaranteed binary orbit (two bodies orbiting each other) ---
    // This overwrites the first two particles so you always see a stable pair.
    if (makeBinary && particles.size() >= 2) {
        const double G = 6.67430e-11;

        Particle* a = particles[0];
        Particle* b = particles[1];

        double m1 = a->getMass();
        double m2 = b->getMass();

        double d = 2.0e10;

        double r1 = d * (m2 / (m1 + m2));
        double r2 = d * (m1 / (m1 + m2));

        a->setPosition(Vector3(-r1, 0, 0));
        b->setPosition(Vector3( r2, 0, 0));

        double vRel = std::sqrt(G * (m1 + m2) / d);

        double v1 = vRel * (m2 / (m1 + m2));
        double v2 = vRel * (m1 / (m1 + m2));

        a->setVelocity(Vector3(0, -v1, 0));
        b->setVelocity(Vector3(0,  v2, 0));
    }

    delete velocityVerlet;
    velocityVerlet = new VelocityVerlet();
}

System::~System() {
    delete velocityVerlet;
}