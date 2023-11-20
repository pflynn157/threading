#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Create an array of numbers 1-10
    int total = 12;
    int array_size = total / size;
    printf("Array Size: %d | Per Proc: %d\n", total, array_size);
    
    int *all_numbers = malloc(sizeof(int) * total);
    int *numbers = malloc(sizeof(int) * array_size);
    
    //
    // Root process, load the array
    //
    if (rank == 0) {
        printf("CPU %d = [", rank);
        for (int i = 0; i<total; i++) {
            all_numbers[i] = i;
            printf("%d ", i);
        }
        printf("]\n");
    }
    
    //
    // The computation
    //
    // 0 = root
    // array_size = equal for all cpus so they have the same data, and so the root does work
    MPI_Scatter(all_numbers, array_size, MPI_INT, numbers, array_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Print
    printf("\tCPU: %d = [", rank);
    for (int i = 0; i<array_size; i++) printf("%d ", numbers[i]);
    printf("]\n");
    
    // Multiply each number by 10
    for (int i = 0; i<array_size; i++) {
        numbers[i] *= 10;
    }
    
    // Send back to the root process
    MPI_Gather(numbers, array_size, MPI_INT, all_numbers, array_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    //
    // When we're done, show the final result
    //
    if (rank == 0) {
        printf("DONE = [");
        for (int i = 0; i<total; i++) {
            printf("%d ", all_numbers[i]);
        }
        printf("]\n");
    }
    
    MPI_Finalize();
}

