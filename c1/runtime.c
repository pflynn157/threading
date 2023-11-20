#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "runtime.h"

void __rt_fork_call(int thread_count, rt_func func, int argc, ...) {
    pthread_t thread_pool[thread_count];
    for (int i = 0; i<thread_count; i++) {
        pthread_t thread;
        thread_pool[i] = thread;
    }
    
    for (int i = 0; i<thread_count; i++) {
        pthread_create(&thread_pool[i], NULL, func, i);
    }
    
    for (int i = 0; i<thread_count; i++) {
        pthread_join(thread_pool[i], NULL);
    }
}

