#ifndef OCTREE_H
#define OCTREE_H

#include "Particle.h"
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>

class Octree{
    private:
        Vector3 center;
        double halfSize;
        
        double mass;
        Vector3 com;

        Particle* body;

        std::array<Octree*,8> children;
    public:
        Octree(Vector3 center, double halfSize);
        Octree();

        bool isLeaf(); // Checks if it has no children

        void insert(Particle* particle);
        void computeMass();
        void clear();
        Vector3 accelOn(const Particle* p, double theta, double G, double eps);

        int childIndex(const Vector3& pos);
        Vector3 childCenter(int idx);
        void subdivide();

        ~Octree();
};


#endif