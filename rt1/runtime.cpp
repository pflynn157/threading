#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>

#define NUM_THREADS 4

struct rt_data {
    // For parallel for loops
    int *start_bounds;
    int *end_bounds;
    int step;
    
    // For the thread manager
    std::vector<std::thread> thread_pool;
};

static rt_data *data;

void init_forloop(int threads, int start, int end, int step) {
    data->start_bounds = new int(threads);
    data->end_bounds = new int(threads);
    data->step = step;

    int part_step = (end - start) / threads;
    part_step += (end - start) % threads;
    int index = 0;
    
    for (int i = start; i<end; i+=part_step) {
        int step_end = i+part_step;
        if (step_end > end) {
            step_end = end;
        }
        
        data->start_bounds[index] = i;
        data->end_bounds[index] = step_end;
        ++index;
    }
}

void init_for_bounds(int tid, int *start, int *end, int *step) {
    *step = 1;
    *start = data->start_bounds[tid];
    *end = data->end_bounds[tid];
}

void fork_call(void (*outlined)(int)) {

}

//
// User code
//
void outlined(int tid) {
    int start = 0;
    int end = 0;
    int step = 0;
    init_for_bounds(tid, &start, &end, &step);
    
    for (int i = start; i<end; i+=step) {
        std::cout << i << std::endl;
    }
}

int main(int argc, char **argv) {
    data = new rt_data();
    init_forloop(NUM_THREADS, 0, 10, 1);
    
    
    for (int i = 0; i<NUM_THREADS; i++) {
        std::thread t(outlined, i);
        data->thread_pool.push_back(std::move(t));
    }
    
    for (int i = 0; i<data->thread_pool.size(); i++) {
        data->thread_pool[i].join();
    }
    
    return 0;
}

