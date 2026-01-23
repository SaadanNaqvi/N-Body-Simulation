#ifndef FORCE_WORKER_H
#define FORCE_WORKER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <functional>

class ForceWorker{
    private:
        void workerLoop();

        int T = 1;
        int bg = 0;
        std::vector<std::thread> threads;

        std::mutex m;
        std::condition_variable cvStart;
        std::condition_variable cvDone;

        bool quit = false;

        int jobBegin = 0, jobEnd = 0;
        std::function<void(int)> jobFn;

        std::atomic<int> nextIndex{0};   
        int jobGen = 0;               
        int remaining = 0;
   
    public:
        ForceWorker(int numThreads = 0);
        ~ForceWorker();

        // Run fn(i) for i in [begin, end)
        void parallelFor(int begin, int end, const std::function<void(int)>& fn);
};

#endif
