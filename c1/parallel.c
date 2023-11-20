//
// Tests a simple parallel function
//
#include <stdio.h>

#include "runtime.h"

void outlined(int tid, ...) {
    printf("Hello from thread: %d\n", tid);
}

int main() {
    __rt_fork_call(4, outlined, 0);
    return 0;
}

