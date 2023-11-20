//
// This is a simple matrix-vector multiplication example in MPI
// The keyword here is simple- it is probably not the best design.
// The way it works is:
//      1) Divide all the matrix rows among the ranks
//      2) Broadcast the vector across all the ranks
//      3) Divide the result vector across the ranks
//      4) Multiply each local row by the vector, and store to the local vector
//      5) Gather the local vector into a single vector
// At the end, we do a serial multiplication and check the results
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <mpi.h>

#define N 32768

void random_fill(int *A, size_t size) {
    for (size_t i = 0; i<size; i++) {
        A[i] = rand() % 10;
    }
}

void zero_fill(int *A, size_t size) {
    for (size_t i = 0; i<size; i++) A[i] = 0;
}

void matvec_serial(int *A, int *B, int *C) {
    for (size_t i = 0; i<N; i++) {
        int tmp = 0;
        for (size_t j = 0; j<N; j++) {
            tmp += A[i*N+j] * B[j];
        }
        C[i] = tmp;
    }
}

int verify(int *C1, int *C2) {
    int errors = 0;
    for (size_t i = 0; i<N; i++) {
        if (C1[i] != C2[i]) ++errors;
    }
    return errors;
}

// Forward declarations
void print(int *A, int *B, int *C);
void print_matrix_part(int *A_local, int rank, size_t matrix_part_size);
void print_vector_part(int *V_local, int rank, size_t part_size, char *name);

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int world;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    size_t part_size = N / world;
    size_t matrix_part_size = (N * N) / world;
    
    // Allocate the main arrays
    int *A = malloc(sizeof(int)*N*N);
    int *B = malloc(sizeof(int)*N);
    int *C = malloc(sizeof(int)*N);
    
    // Fill them
    if (rank == 0) {
        random_fill(A, N*N);
        random_fill(B, N);
        zero_fill(C, N);
    }
    
    //
    // Worker section
    //
    
    // 1) Create worker arrays
    int *A_local = malloc(sizeof(int)*matrix_part_size);
    int *C_local = malloc(sizeof(int)*part_size);
    zero_fill(C_local, part_size);
    
    // 2) Scatter rows across the nodes
    MPI_Scatter(A, matrix_part_size, MPI_INT, A_local, matrix_part_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    // 3) Broadcast the B vector
    MPI_Bcast(B, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Print
    //print_matrix_part(A_local, rank, matrix_part_size);
    //print_vector_part(B, rank, N, "B");
    
    // 4) Begin the multiplication
    for (size_t i = 0; i<part_size; i++) {
        for (size_t j = 0; j<N; j++) {
            C_local[i] += A_local[i*N+j] * B[j];
        }
    }
    
    //print_vector_part(C_local, rank, part_size, "C");
    
    // 5) Gather the C vector back
    MPI_Gather(C_local, part_size, MPI_INT, C, part_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    //
    // Finalize by checking and printing our results
    //
    if (rank == 0) {
        int *A_serial = malloc(sizeof(int)*N*N);
        int *B_serial = malloc(sizeof(int)*N);
        int *C_serial = malloc(sizeof(int)*N);
        memcpy(A_serial, A, sizeof(int)*N*N);
        memcpy(B_serial, B, sizeof(int)*N);
        memcpy(C_serial, C, sizeof(int)*N);
    
        matvec_serial(A_serial, B_serial, C_serial);
        //print(A, B, C);
        //print(A_serial, B_serial, C_serial);
        
        printf("Verify (ideal = 0): %d\n", verify(C, C_serial));
    }

    MPI_Finalize();
}

//
// All our print functions are down here
//
void print(int *A, int *B, int *C) {
    puts("------------------------------------------");
    for (size_t i = 0; i<N; i++) {
        printf("[");
        for (size_t j = 0; j<N; j++) {
            printf("%d ", A[i*N+j]);
        }
        printf("] * [%d] = [%d]\n", B[i], C[i]);
    }
    puts("------------------------------------------");
}

void print_matrix_part(int *A_local, int rank, size_t matrix_part_size) {
    printf("\tCPU: %d = A[", rank);
    int idx = 0;
    for (size_t i = 0; i<matrix_part_size; i++) {
        printf("%d ", A_local[i]);
        if (idx == N - 1) {
            printf(" | ");
            idx = 0;
        } else {
            ++idx;
        }
    }
    printf("]\n");
}

void print_vector_part(int *V_local, int rank, size_t part_size, char *name) {
    printf("\tCPU: %d = %s[", rank, name);
    for (size_t i = 0; i<part_size; i++) printf("%d ", V_local[i]);
    printf("]\n");
}

