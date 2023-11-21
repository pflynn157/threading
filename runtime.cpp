#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <memory>
#include <vector>

extern "C" {        // We need this to be a C library

static int threads;

void __rt_fork_call(int thread_count, void(*func)(int tid, ...), int argc, ...) {
    threads = thread_count;
    std::vector<std::thread> thread_pool;
    for (int i = 0; i<thread_count; i++) {
        std::thread thread(func, i);
        thread_pool.push_back(std::move(thread));
    }
    
    for (int i = 0; i<thread_count; i++) {
        thread_pool[i].join();
    }
}

//
// Partitions for loops
//
void __rt_init_for(int tid, int *start, int *end, int *step) {
    int end1 = *end;
    int part_size = (*end / threads) + (*end % threads);
    *start = tid * part_size;
    *end = *start + part_size;
    if (*end > end1) *end = end1;
}

} // end extern C

