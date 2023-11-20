#define NUM_THREADS 3

void outlined(int tid, rt_data *data) {
    int start = 0;
    int end = 0;
    int step = 0;
    init_for_bounds(tid, data, &start, &end, &step);
    
    for (int i = start; i<end; i+=step) {
        std::cout << i << std::endl;
    }
}

int main(int argc, char **argv) {
    rt_data *data = new rt_data();
    init_forloop(data, NUM_THREADS, 0, 10, 1);
    std::vector<std::thread> thread_pool;
    
    for (int i = 0; i<NUM_THREADS; i++) {
        std::thread t(outlined, i, data);
        thread_pool.push_back(std::move(t));
    }
    
    for (int i = 0; i<thread_pool.size(); i++) {
        thread_pool[i].join();
    }
    
    return 0;
}
