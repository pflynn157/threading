#pragma once

typedef void(*rt_func)(int tid, ...);

void __rt_fork_call(int thread_count, rt_func func, int argc, ...) ;

