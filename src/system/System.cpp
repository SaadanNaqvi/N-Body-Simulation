#include "System.h"

System::System(){}

using Clock = std::chrono::steady_clock;

inline Clock::time_point now(){
    return Clock::now();
}

inline double ms(Clock::time_point start, Clock::time_point end){
    return std::chrono::duration<double, std::milli>(end - start).count();
}



static void computeSceneBounds(const std::vector<Particle*>& ps, Vector3& center, double& halfSize){
    if (ps.empty()){
        center = Vector3(0,0,0);
        halfSize = 1.0;
        return;
    }

    double minX =  1e300, minY =  1e300, minZ =  1e300;
    double maxX = -1e300, maxY = -1e300, maxZ = -1e300;

    for (auto* p : ps){
        Vector3 x = p->getPosition();
        minX = std::min(minX, x.getX()); maxX = std::max(maxX, x.getX());
        minY = std::min(minY, x.getY()); maxY = std::max(maxY, x.getY());
        minZ = std::min(minZ, x.getZ()); maxZ = std::max(maxZ, x.getZ());
    }

    center = Vector3(
        0.5*(minX + maxX),
        0.5*(minY + maxY),
        0.5*(minZ + maxZ)
    );

    double spanX = maxX - minX;
    double spanY = maxY - minY;
    double spanZ = maxZ - minZ;

    double span = std::max({spanX, spanY, spanZ});
    halfSize = 0.5 * span;
   
    halfSize = std::max(halfSize * 1.25, 1.0);  // padding so that the bodies on the edge still fit
}



System::System(std::vector<Particle*> particles){
    this->particles = particles;
    velocityVerlet = new VelocityVerlet();
}

void System::addParticle(Particle* particle){
    this->particles.push_back(particle);
    velocityVerlet = new VelocityVerlet();
}

void System::update(double dt){
    const int n = particles.size();
    if (n == 0) return;

    const double theta = 2.5;
    const double G = 6.67430e-11;
    const double eps = 1e5;

    if ((int)acc.size() != n) acc.assign(n, Vector3(0,0,0));
    if ((int)netForcesVec.size() != n) netForcesVec.assign(n, Vector3(0,0,0));

    Vector3 sceneCenter; double sceneHalfSize;
    computeSceneBounds(particles, sceneCenter, sceneHalfSize);
    octree.reset(sceneCenter, sceneHalfSize);
    for (auto* p : particles) octree.insert(p);

    worker.parallelFor(0, n, [&](int i){
        acc[i] = octree.accelOn(particles[i], theta, G, eps);
    });

    worker.parallelFor(0, n, [&](int i){
        Particle* p = particles[i];
        Vector3 x = p->getPosition();
        Vector3 v = p->getVelocity();
        p->setPosition(x + v*dt + acc[i]*(0.5*dt*dt));
    });

    computeSceneBounds(particles, sceneCenter, sceneHalfSize);
    octree.reset(sceneCenter, sceneHalfSize);
    for (auto* p : particles) octree.insert(p);

    worker.parallelFor(0, n, [&](int i){
        Vector3 aNew = octree.accelOn(particles[i], theta, G, eps);

    
        Particle* p = particles[i];
        Vector3 v = p->getVelocity();
        p->setVelocity(v + (acc[i] + aNew)*(0.5*dt));

        acc[i] = aNew;
    });
}





std::vector<Particle*>& System::getParticles(){
    return this->particles;
}


GravityForce& System::getGravityForce(){
    return this->gravityForce;
}


void System::randomSpawn(){
    for (Particle* p : particles) delete p;
    particles.clear();

    static std::mt19937 rng(std::random_device{}());

    const int N = 10000;      
    const double region = 8e11; 
    const double maxSpeed = 15000.0; 
    const double mMin = 1e20;       
    const double mMax = 5e28;        
    const double radiusBase = 5e3;    
    const bool makeBinary = true;      

    std::uniform_real_distribution<double> posDist(-region, region);
    std::uniform_real_distribution<double> speedDist(-maxSpeed, maxSpeed);
    std::uniform_real_distribution<double> massDist(mMin, mMax);

    particles.reserve(N);

    for (int i = 0; i < N; i++){
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
        double vEsc = std::sqrt(2.0 * G * (m1 + m2) / d);
        double vRel = 1.2 * vEsc;

        double v1 = vRel * (m2 / (m1 + m2));
        double v2 = vRel * (m1 / (m1 + m2));

        a->setVelocity(Vector3(0, -v1, 0));
        b->setVelocity(Vector3(0,  v2, 0));
    }

    delete velocityVerlet;
    velocityVerlet = new VelocityVerlet();
}


void System::computeAccelerationsBH(std::vector<Vector3>& outAcc,double theta, double G, double eps){
    int n = particles.size();
    outAcc.resize(n);

    Vector3 sceneCenter; double sceneHalfSize;
    computeSceneBounds(particles, sceneCenter, sceneHalfSize);

    octree.reset(sceneCenter, sceneHalfSize);
    for (auto* p : particles) octree.insert(p);

    worker.parallelFor(0, n, [&](int i){
        outAcc[i] = octree.accelOn(particles[i], theta, G, eps);
    });
}


System::~System() {
    delete velocityVerlet;
}