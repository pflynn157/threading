#pragma once

void __rt_fork_call(int thread_count, void(*func)(int tid, ...), int argc, ...);
void __rt_init_for(int tid, int *start, int *end, int *step);


