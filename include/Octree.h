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

        std::vector<Particle *> bodies;

        std::array<Octree *, 8> children;

        static constexpr int BUCKET = 8;

        int childIndex(const Vector3 &pos) const;
        Vector3 childCenter(int index) const;
        void subdivide();
        void pushBodyToChild(Particle *p);

        Vector3 accelDirectLeaf(Particle *p, double G, double eps) const;

    public:
        static inline long long visits = 0;
        static inline long long approximations = 0;

        Octree(Vector3 center, double halfSize);
        Octree();

        bool isLeaf() const;
        void clear();

        void insert(Particle *p);
        Vector3 accelOn(Particle *p, double theta, double G, double eps) const;
        
        void reset(Vector3 c, double hs);

        ~Octree();   
};

#endif
