//
// Tests a simple parallel function
//
#include <iostream>

#include "runtime.h"

void outlined(int tid, ...) {
    std::cout << "Thread: " << tid << std::endl;
}

int main() {
    __rt_fork_call(4, outlined, 0);
    return 0;
}

