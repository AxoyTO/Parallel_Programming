#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

void roundRobin(const int rank, const int world_size){
    int prev_rank = rank == 0 ? world_size - 1 : rank - 1;
    int next_rank = rank == world_size - 1 ? 0 : rank + 1;
    srand(time(NULL) + rank);
    int random = rand() % 1000;
    MPI_Status status;
    //printf("(%d) <- (%d) -> (%d)\n", prev_rank, rank, next_rank);
    printf("P:(%d) Generated %d and sent to P:(%d)\n", rank, random, next_rank);
    MPI_Sendrecv(&random, 1, MPI_INT, next_rank, 0, &random, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
    //MPI_Barrier(MPI_COMM_WORLD);
    printf("P:(%d) Received %d from P:(%d)\n", rank, random, prev_rank);
}

int main(int argc, char **argv){
    MPI_Init(NULL, NULL);
    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //printf("(%d/%d): Hello, World!\n", rank, world_size);
    roundRobin(rank, world_size);

    MPI_Finalize();
    return 0;
}
