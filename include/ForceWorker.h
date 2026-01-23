#ifndef FORCE_WORKER_H
#define FORCE_WORKER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "Octree.h"

class ForceWorker{
    private:
        void threadMain();

        std::thread worker;

        std::mutex forceMutex;
        std::condition_variable cv;
        
        bool hasJob = false;
        bool done = false;
        bool quit = false;

        Octree* tree = nullptr;
        std::vector<Particle*>* particles = nullptr;
        std::vector<Vector3>* outAcc = nullptr;
        int begin = 0;
        int end = 0;
        double theta = 0.0;
        double G = 0.0;
        double eps = 0.0;

    public:
        ForceWorker();

        void startJob(Octree* tree, std::vector<Particle*>* particles, std::vector<Vector3>* outAcc, int begin, int end, double theta, double G, double eps);
        void wait();

        ~ForceWorker();

};


#endif  