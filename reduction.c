//
// Tests a simple parallel function
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "runtime.h"

void outlined(int tid, ...) {
    int start = 0;
    int end = 20;
    int step = 1;
    __rt_init_for(tid, &start, &end, &step);
    for (int i = start; i<end; i++) {
        printf("%d\n", A[i]);
    }
}

int main() {
    int *A = malloc(sizeof(int)*20);
    for (int i = 0; i<20; i++) A[i] = i + 1;
    
    __rt_fork_call(4, outlined, 1, A);
    return 0;
}

