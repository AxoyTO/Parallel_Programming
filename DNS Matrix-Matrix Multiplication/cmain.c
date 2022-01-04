//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

//using namespace std;

void printAll(int *mat, int nloc, MPI_Comm comm, char *label){
    int nbPE, globalPE;
    MPI_Comm_size(comm, &nbPE);
    MPI_Comm_rank(MPI_COMM_WORLD, &globalPE);
    int *recv_buf = (int *) malloc(sizeof(int) * nbPE * nloc * nloc);

    MPI_Gather(mat, nloc * nloc, MPI_INT, recv_buf, nloc * nloc, MPI_INT, 0, comm);
    if(globalPE == 0){
        int p = sqrt(nbPE + 0.1);
        printf("%s", label);
        for(int global = 0; global < (p * nloc) * (p * nloc); global++){
            int global_i = global / (p * nloc);
            int global_j = global % (p * nloc);
            int pe_i = global_i / nloc;
            int pe_j = global_j / nloc;
            int local_i = global_i % nloc;
            int local_j = global_j % nloc;
            int pe = pe_i * p + pe_j;
            int local = local_i * nloc + local_j;
            printf("%d ", recv_buf[pe * nloc * nloc + local]);
            if((global + 1) % (p * nloc) == 0)
                printf("\n");
        }
    }
}

int *randomInit(int size, int inf, int sup){
    int *mat = (int *) malloc(size * size * sizeof(int));
    for(int i = 0; i < size * size; i++) mat[i] = inf + rand() % (sup - inf);
    return mat;
}

void matrixMultiplication(const int *a, const int *b, int *resultat, int nloc){
    for(int i = 0; i < nloc; i++){
        for(int j = 0; j < nloc; j++){
            for(int k = 0; k < nloc; k++){
                resultat[i * nloc + j] += a[i * nloc + k] * b[k * nloc + j];
            }
        }
    }
}

void dns(int *matLocA, int *matLocB, int *matLocC, int nloc){
    int nbPE, myPE;
    MPI_Comm_size(MPI_COMM_WORLD, &nbPE);
    MPI_Comm_rank(MPI_COMM_WORLD, &myPE);

    int p = pow(nbPE + 0.1, 1.0 / 3.0);

    MPI_Comm comm_a, comm_b, comm_c;
    MPI_Comm_split(MPI_COMM_WORLD, myPE % p + ((myPE / (p * p)) * p), myPE, &comm_a);
    MPI_Comm_split(MPI_COMM_WORLD, myPE % (p * p), myPE, &comm_b);
    MPI_Comm_split(MPI_COMM_WORLD, myPE / p, myPE, &comm_c);

    MPI_Bcast(matLocA, nloc * nloc, MPI_INT, 0, comm_a);
    MPI_Bcast(matLocB, nloc * nloc, MPI_INT, 0, comm_b);

    int *result = (int *) malloc(nloc * nloc * sizeof(int));
    for(int j = 0; j < nloc * nloc; ++j){
        result[j] = 0;
    }
    matrixMultiplication(matLocA, matLocB, result, nloc);

    MPI_Reduce(result, matLocC, nloc * nloc, MPI_INT, MPI_SUM, 0, comm_c);
}

int main(int argc, char **argv){
    double start, end;
    MPI_Init(&argc, &argv);
    int nbPE, myPE;
    MPI_Comm_size(MPI_COMM_WORLD, &nbPE);
    MPI_Comm_rank(MPI_COMM_WORLD, &myPE);
    MPI_Comm comm_i_cte = MPI_COMM_WORLD,
            comm_j_cte = MPI_COMM_WORLD,
            comm_k_cte = MPI_COMM_WORLD;

    int nloc = atoi(argv[1]);

    int *matLocA = randomInit(nloc, -10, 10);
    int *matLocB = randomInit(nloc, -10, 10);
    int *matLocC = (int *) malloc(nloc * nloc * sizeof(int));


    start = MPI_Wtime();

    int p = pow(nbPE + 0.1, 1.0 / 3.0);
    int j = (myPE / p) % p;
    MPI_Comm_split(MPI_COMM_WORLD, j, myPE, &comm_j_cte);
    int i = myPE / (p * p);
    MPI_Comm_split(MPI_COMM_WORLD, i, (myPE % p) * p + myPE / p, &comm_i_cte);
    int k = myPE % p;
    MPI_Comm_split(MPI_COMM_WORLD, k, myPE, &comm_k_cte);


    dns(matLocA, matLocB, matLocC, nloc);
    end = MPI_Wtime();

    printAll(matLocA, nloc, comm_j_cte, "%matrice complete A\n");
    printAll(matLocB, nloc, comm_i_cte, "%matrice complete B\n");
    printAll(matLocC, nloc, comm_k_cte, "%matrice complete C\n");
    printf("Elapsed Time: %f\n", end - start);

    MPI_Finalize();

    return 0;
}