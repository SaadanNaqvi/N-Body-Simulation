#include "ForceWorker.h"

ForceWorker::ForceWorker(){
    worker = std::thread(&ForceWorker::threadMain, *this);
}

void ForceWorker::startJob(Octree* tree, std::vector<Particle*>* particles, std::vector<Vector3>* outAcc, int begin, int end, double theta, double G, double eps){
    std::lock_guard<std::mutex> lock(forceMutex);
    this->tree = tree;
    this->particles = particles;
    this->outAcc = outAcc;
    this->begin = begin;
    this->end = end;
    this->theta = theta;
    this->G = G;
    this->eps = eps;

    done = false;
    hasJob = true;

    cv.notify_one();
}

void ForceWorker::wait(){
    std::unique_lock<std::mutex> lock(forceMutex);
    cv.wait(lock, [&]{
        return done;
    });
}

void ForceWorker::threadMain(){
    std::unique_lock<std::mutex> lock(forceMutex);

    while(true){
        cv.wait(lock, [&]{
            return hasJob;
        });

        if(quit) break;
        const Octree* t = tree;
        const auto* ps = particles;
        auto* acc = outAcc;
        int L = begin;
        int R = end;
        double th = theta;
        double g = G;
        double e = eps;

        hasJob = false;
        lock.unlock();

        // The main worker its meant to do
        for (int i = L; i < R; i++){
            (*acc)[i] = t->accelOn((*ps)[i], th, g, e);
        }
        lock.lock();
        done = true;
        cv.notify_one();
    }
}



ForceWorker::~ForceWorker(){
    {
        std::lock_guard<std::mutex> lock(forceMutex);
        quit=true;
        hasJob = true;
    }
    cv.notify_one();
    worker.join();
}