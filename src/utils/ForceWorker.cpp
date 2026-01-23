#include "ForceWorker.h"
#include <algorithm>

static int defaultThreads(){
    unsigned hc = std::thread::hardware_concurrency();
    if (hc == 0) hc = 4;
    return (int)hc;
}

ForceWorker::ForceWorker(int numThreads) {
    T = (numThreads <= 0) ? defaultThreads() : numThreads;
    T = std::max(1, T);
    bg = std::max(0, T - 1);

    threads.reserve(bg);
    for (int i = 0; i < bg; i++) {
        threads.emplace_back(&ForceWorker::workerLoop, this);
    }
}

void ForceWorker::parallelFor(int begin, int end, const std::function<void(int)>& fn){
    if (end <= begin) return;

    {
        std::lock_guard<std::mutex> lock(m);
        jobBegin = begin;
        jobEnd   = end;
        jobFn    = fn;

        nextIndex.store(begin, std::memory_order_relaxed);

        remaining = bg;     // number of background threads that must finish
        jobGen++;       
    }

    cvStart.notify_all();

    while (true){
        int i = nextIndex.fetch_add(1, std::memory_order_relaxed);
        if (i >= end) break;
        fn(i);
    }

    // wait for background workers
    std::unique_lock<std::mutex> lock(m);
    cvDone.wait(lock, [&] { return remaining == 0; });
}

void ForceWorker::workerLoop(){
    int localGen = 0;

    std::unique_lock<std::mutex> lock(m);
    while (true) {
        cvStart.wait(lock, [&] { return quit || jobGen != localGen; });
        if (quit) break;

        localGen = jobGen;
        int end = jobEnd;
        auto fn = jobFn;

        lock.unlock();

        while (true) {
            int i = nextIndex.fetch_add(1, std::memory_order_relaxed);
            if (i >= end) break;
            fn(i);
        }

        lock.lock();

        remaining--;
        if (remaining == 0) {
            cvDone.notify_one();
        }
    }
}

ForceWorker::~ForceWorker(){
    {
        std::lock_guard<std::mutex> lock(m);
        quit = true;
    }
    cvStart.notify_all();
    for (auto& t : threads) t.join();
}
