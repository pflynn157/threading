#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t **address_list;
static size_t address_count;

void gc_init() {
    address_list = malloc(sizeof(uint64_t*)*20);
    address_count = 0;
}

char *gc_alloc_i8(int size) {
    char *ptr = malloc(sizeof(char)*size);
    address_list[address_count] = ptr;
    ++address_count;
    return ptr;
}

int *gc_alloc_i32(int size) {
    int *ptr = malloc(sizeof(int)*size);
    address_list[address_count] = ptr;
    ++address_count;
    return ptr;
}

uint64_t *gc_alloc_i64(int size) {
    uint64_t *ptr = malloc(sizeof(uint64_t)*size);
    address_list[address_count] = ptr;
    ++address_count;
    return ptr;
}

void gc_destroy() {
    for (int i = 0; i<address_count; i++) {
        free(address_list[i]);
    }
    free(address_list);
}

