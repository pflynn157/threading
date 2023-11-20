#include <stdio.h>
#include <mpi.h>

/*************************
MPI_Send(
    void* data,
    int count,
    MPI_Datatype datatype,
    int destination,
    int tag,
    MPI_Comm communicator)
*/

/*************************
MPI_Recv(
    void* data,
    int count,
    MPI_Datatype datatype,
    int source,
    int tag,
    MPI_Comm communicator,
    MPI_Status* status)
*/

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int world;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int number = 0;
    if (rank == 0) {
        printf("Parent- will send data.\n");
        number = 52;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        printf("Child %d- will receive data.\n", rank);
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("\tRECV data: %d\n", number);
    }
    
    MPI_Finalize();
}

