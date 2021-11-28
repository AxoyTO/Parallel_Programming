#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

//Should be defined !=0 to work with await_request();
#define BUSY_WAIT 0

time_t start;

void start_timer(){
    start = clock();
}

float get_timer(){
    time_t end = clock();
    return (float) (end - start) / CLOCKS_PER_SEC;
}

void debug(int rank, const char *format, ...){
    va_list(args);
    va_start(args, format);

    printf("%.6f secs | P:%2d | ", get_timer(), rank);
    vprintf(format, args);

    va_end(args);

}


void await_request(int rank, MPI_Request *request){
#if BUSY_WAIT
    int wait_count = 0;
    int flag = 0;
    MPI_Status status;

    do{
        wait_count++;
        MPI_Test(request, &flag, &status);
    } while(!flag);
    debug(rank, "tested %d times!\n", wait_count);
#endif
}


void send_to_many(int rank, int comm_size){
    int random = rand() % 10;
    debug(rank, "Random value is: %d\n", random);
    MPI_Request send_request;

    for(int to_rank = 0; to_rank < comm_size; to_rank++){
        if(to_rank == rank)
            continue;
        int value_sent = random + to_rank;

        MPI_Isend(&value_sent, 1, MPI_INT, to_rank, 0, MPI_COMM_WORLD, &send_request);

        await_request(rank, &send_request);
        debug(rank, "Sent %d to P:%d\n", value_sent, to_rank);
    }
}

void receive_from_one(int sender, int rank){
    int recv_buf;
    MPI_Request recv_request;

    MPI_Irecv(&recv_buf, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, &recv_request);

    await_request(sender, &recv_request);
    debug(rank, "got value %d\n", recv_buf);
}


int main(){
    int comm_size;
    int rank, sender = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL) + rank);
    //debug(rank, "P:%d Hello!\n", rank);
    start_timer();
    if(rank == 0){
        sender = rand() % comm_size;
        debug(sender, "is sending\n\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == sender){
        send_to_many(rank, comm_size);
    } else{
        receive_from_one(sender, rank);
    }
    //debug(rank, "P:%d Bye\n", rank);
    MPI_Finalize();
    return 0;
}
