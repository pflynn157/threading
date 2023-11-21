//
// Tests a simple parallel function
//
#include <stdio.h>

#include "runtime.h"

void outlined(int tid, ...) {
    int start = 0;
    int end = 22;
    int step = 1;
    __rt_init_for(tid, &start, &end, &step);
    for (int i = start; i<end; i++) {
        printf("%d\n", i);
    }
}

int main() {
    __rt_fork_call(4, outlined, 0);
    return 0;
}

