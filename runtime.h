#pragma once

#include <stdarg.h>

void __rt_fork_call(int thread_count, void(*func)(int tid, va_list args), int argc, ...);
void __rt_init_for(int tid, int *start, int *end, int *step);


