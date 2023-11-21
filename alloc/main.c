#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void func1() {
    int *A = gc_alloc_i32(10);
}

void func2() {
    char *A = gc_alloc_i8(10);
}

void func3() {
    uint64_t *V = gc_alloc_i64(10);
}

int main() {
    gc_init();
    func1(); func2(); func3();
    gc_destroy();
    return 0;
}

