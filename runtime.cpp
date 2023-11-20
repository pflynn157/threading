#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <memory>
#include <vector>

#include "runtime.h"

void __rt_fork_call(int thread_count, rt_func func, int argc, ...) {
    std::vector<std::thread> thread_pool;
    for (int i = 0; i<thread_count; i++) {
        std::thread thread(func, i);
        thread_pool.push_back(std::move(thread));
    }
    
    for (int i = 0; i<thread_count; i++) {
        thread_pool[i].join();
    }
}

