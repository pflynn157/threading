#!/bin/bash

g++ -fPIC -c runtime.cpp -o runtime.o
g++ -shared -o libruntime.so -lpthread \
    runtime.o

gcc parallel.c -o prt -L. -lruntime #-lpthread -g
gcc parfor.c -o parfor -L. -lruntime #-lpthread -g
#gcc reduction.c -o rd -L. -lruntime

