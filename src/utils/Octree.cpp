#include "Octree.h"

Octree::Octree(Vector3 c, double hs)
    : center(c), halfSize(hs), mass(0.0), com(0,0,0), bodies() {
    children.fill(nullptr);
    bodies.reserve(BUCKET);
}

Octree::Octree()
    : center(0,0,0), halfSize(0.0), mass(0.0), com(0,0,0), bodies() {
    children.fill(nullptr);
    bodies.reserve(BUCKET);
}

bool Octree::isLeaf() const {
    for (auto* ch : children) if (ch) return false;
    return true;
}

int Octree::childIndex(const Vector3& pos) const {
    int idx = 0;
    if (pos.getX() >= center.getX()) idx |= 1;
    if (pos.getY() >= center.getY()) idx |= 2;
    if (pos.getZ() >= center.getZ()) idx |= 4;
    return idx;
}

Vector3 Octree::childCenter(int idx) const {
    double q = halfSize * 0.5;
    double cx = center.getX() + ((idx & 1) ? +q : -q);
    double cy = center.getY() + ((idx & 2) ? +q : -q);
    double cz = center.getZ() + ((idx & 4) ? +q : -q);
    return Vector3(cx, cy, cz);
}

void Octree::subdivide() {
    if (!isLeaf()) return;
    double q = halfSize * 0.5;
    for (int i = 0; i < 8; i++) {
        children[i] = new Octree(childCenter(i), q);
    }
}

void Octree::pushBodyToChild(Particle* p) {
    int ci = childIndex(p->getPosition());
    children[ci]->insert(p);
}

void Octree::insert(Particle* p) {
    if (!p) return;

    const Vector3 pos = p->getPosition();
    const double m = p->getMass();

    // Update this node's mass + COM incrementally
    const double mOld = mass;
    const double mNew = mOld + m;
    if (mNew > 0.0) {
        com = (com * mOld + pos * m) / mNew;
        mass = mNew;
    }

    if (isLeaf() && (int)bodies.size() < BUCKET) {
        bodies.push_back(p);
        return;
    }

    // If leaf but full: subdivide and push existing bodies down
    if (isLeaf()) {
        subdivide();
        for (Particle* b : bodies) pushBodyToChild(b);
        bodies.clear(); 
    }

    pushBodyToChild(p);
}

Vector3 Octree::accelDirectLeaf(Particle* p, double G, double eps) const {
    Vector3 acc(0,0,0);
    const Vector3 pos = p->getPosition();

    for (Particle* b : bodies) {
        if (b == p) continue;

        const Vector3 bp = b->getPosition();
        const double dx = bp.getX() - pos.getX();
        const double dy = bp.getY() - pos.getY();
        const double dz = bp.getZ() - pos.getZ();

        const double r2 = dx*dx + dy*dy + dz*dz + eps*eps;
        const double invR = 1.0 / std::sqrt(r2);
        const double invR3 = invR * invR * invR;

        const double a = G * b->getMass() * invR3;
        acc += Vector3(dx * a, dy * a, dz * a);
    }
    return acc;
}

Vector3 Octree::accelOn(Particle* p, double theta, double G, double eps) const {
    //Octree::visits++;

    if (!p) return Vector3(0,0,0);
    if (mass <= 0.0) return Vector3(0,0,0);

    const Vector3 pos = p->getPosition();
    const double dx = com.getX() - pos.getX();
    const double dy = com.getY() - pos.getY();
    const double dz = com.getZ() - pos.getZ();

    double r2_geom = dx*dx + dy*dy + dz*dz;  
    double r2_soft = r2_geom + eps*eps;     

    if (isLeaf()) {
        return accelDirectLeaf(p, G, eps);
    }

    // Barnes Hut dudes criterion using squared form
    const double s = 2.0 * halfSize;
    const double s2 = s * s;
    const double theta2 = theta * theta;

    if (s2 < theta2 * r2_geom) {
        //Octree::approximations++;
        const double invR = 1.0 / std::sqrt(r2_soft);
        const double invR3 = invR * invR * invR;
        const double a = G * mass * invR3;
        return Vector3(dx * a, dy * a, dz * a);
    }

    Vector3 acc(0,0,0);
    for (auto* ch : children) {
        if (!ch) continue;
        if (ch->mass <= 0.0) continue;
        acc += ch->accelOn(p, theta, G, eps);
    }
    return acc;
}

void Octree::clear() {
    mass = 0.0;
    com  = Vector3(0,0,0);
    bodies.clear();

    for (auto*& ch : children) {
        if (ch) {
            ch->clear();
            delete ch;
            ch = nullptr;
        }
    }
}

Octree::~Octree() {
    clear();
}


void Octree::reset(Vector3 c, double hs){
    clear();
    this->center = c;
    this->halfSize = hs;

    mass = 0.0;
    com  = Vector3(0,0,0);

    bodies.clear();
    bodies.reserve(BUCKET);
}
