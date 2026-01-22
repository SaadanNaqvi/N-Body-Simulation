#include "Octree.h"

Octree::Octree(Vector3 center, double halfSize){
    this->center = center;
    this->com = Vector3(0, 0, 0);
    this->mass = 0.0;
    this->body = nullptr;
    this->halfSize = halfSize;

    children.fill(nullptr);
}


Octree::Octree(){
    this->center = Vector3(0, 0, 0);
    this->com = Vector3(0, 0, 0);
    this->mass = 0.0;
    this->body = nullptr;
    this->halfSize = 0.0;

    children.fill(nullptr);
}

bool Octree::isLeaf(){
    for (auto* ch : children) if (ch) return false;
    return true;
}

int Octree::childIndex(const Vector3& pos){
    int index = 0;
    if (pos.getX() >= center.getX()) index |= 1;
    if (pos.getY() >= center.getY()) index |= 2; 
    if (pos.getZ() >= center.getZ()) index |= 4;
    return index;
}

Vector3 Octree::childCenter(int index){
    double q = halfSize * 0.5;

    double cx = center.getX() + ((index & 1) ? +q : -q);
    double cy = center.getY() + ((index & 2) ? +q : -q);
    double cz = center.getZ() + ((index & 4) ? +q : -q);

    return Vector3(cx, cy, cz);
}

void Octree::subdivide(){
    double q = halfSize * 0.5;
    for (int i = 0; i < 8; i++){
        children[i] = new Octree(childCenter(i), q);
    }
}


void Octree::insert(Particle* p){
    if (!p) return;

    Vector3 pos = p->getPosition();

    if (mass == 0.0 && body == nullptr && isLeaf()){
        body = p;
        mass = p->getMass();
        com  = pos;
        return;
    }


    if (isLeaf()){
        Particle* old = body;
        body = nullptr;
        subdivide();

        if (old){
            int oi = childIndex(old->getPosition());
            children[oi]->insert(old);
        }
    }

    int ci = childIndex(pos);
    children[ci]->insert(p);

    double mOld = mass;
    double mNew = mOld + p->getMass();

    if (mNew > 0.0){
        Vector3 weightedOld = com * mOld;
        Vector3 weightedNew = pos * p->getMass();
        com  = (weightedOld + weightedNew) / mNew;
        mass = mNew;
    }
}

void Octree::computeMass(){
    if (isLeaf()){
        if (body){
            mass = body->getMass();
            com  = body->getPosition();
        } else {
            mass = 0.0;
            com  = Vector3(0,0,0);
        }
        return;
    }

    mass = 0.0;
    com  = Vector3(0,0,0);

    for (auto* ch : children){
        if (!ch) continue;
        ch->computeMass();
        if (ch->mass <= 0.0) continue;

        com  += ch->com * ch->mass;
        mass += ch->mass;
    }

    if (mass > 0.0) com = com / mass;
}

Vector3 Octree::accelOn(Particle* p, double theta, double G, double eps){
    Octree::visits++;
    if (!p) return Vector3(0,0,0);
    if (mass <= 0.0) return Vector3(0,0,0);

    if (isLeaf() && body == p) return Vector3(0,0,0); // If this leaf is exactly the same particle, skip (avoid self-force)

    Vector3 pos = p->getPosition();

    double dx = com.getX() -pos.getX();
    double dy = com.getY()- pos.getY();
    double dz = com.getZ() - pos.getZ();

    double r2 = dx*dx + dy*dy + dz*dz + eps*eps;

    // Barnes-Hut criterion: (s / d) < theta
    // Here s = cube width = 2*halfSize, d = distance to COM
    double s = 2.0 * halfSize;
    double s2 = s * s;
    double theta2 = theta * theta;

    if (isLeaf() || s2 < theta2 * r2){
        Octree::approximations++;
        // treat this node as one mass at COM
        double invR = 1.0 / std::sqrt(r2);
        double invR3 = invR*invR * invR;      // 1 / r^3
        double a = G * mass *invR3;        // accel magnitude scale

        return Vector3(dx * a, dy * a, dz * a);
    }

    Vector3 acc(0,0,0);
    for (auto* ch : children){
        if (!ch) continue;
        acc += ch->accelOn(p, theta, G, eps);
    }
    return acc;
}

void Octree::clear(){
    body = nullptr;
    mass=0.0;
    com =Vector3(0,0,0);

    for (auto*& ch : children){
        if (ch){
            ch->clear();
            delete ch;
            ch = nullptr;
        }
    }
}

Octree::~Octree(){
    clear();
}