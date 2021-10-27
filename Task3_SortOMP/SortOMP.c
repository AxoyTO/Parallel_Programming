#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

const int ERROR = 1;

void display_array(int *A, const long n){
    for(int i = 0; i < n; i++){
        printf("%d", *(A + i));
        if(i + 1 != n)
            printf(" ");
    }
    printf("\n");
}

void sort(){

}

int main(int argc, char **argv){
    if(argc == 3){
        long n = atoi(argv[1]);
        int n_threads = atoi(argv[2]);

        int *A = (int *) malloc(sizeof(int) * n);
        if(A == NULL){
            fputs("Failed to allocate memory for A..", stderr);
            return ERROR;
        }

        for(int i = 0; i < n; i++){
            *(A + i) = n - i;
        }
        //display_array(A, n);

#pragma omp parallel num_threads(n_threads) default(none) shared(A, n)
        {
#pragma omp for schedule(static, 6)
            for(int i = 0; i < n; i++){
                printf("%d ", A[i]);
            }
#pragma omp single
            printf("\n");
        }

        //display_array(A, n);

    } else{
        fputs("Command line format:\n", stderr);
        fputs("./<binary> <array_size> <number_of_open_mp_threads>\n", stderr);
        fputs("For example: ./SortOMP 100500 4\n", stdout);
    }

    return 0;
}