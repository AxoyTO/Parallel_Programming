#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>

int compare(const void *a, const void *b){
    return (*(int *) a - *(int *) b);
}

//void display_array(const int *A, const int n){
//    for(int i = 0; i < n; i++){
//        printf("%d", *(A + i));
//        if(i + 1 != n)
//            printf(" ");
//    }
//    printf("\n");
//}

void merge(int A[], int left, int middle, int right){
    int temp[middle - left + 1], temp2[right - middle];
    for(int i = 0; i < (middle - left + 1); i++){
        temp[i] = A[left + i];
    }
    for(int i = 0; i < (right - middle); i++){
        temp2[i] = A[middle + 1 + i];
    }
    int i = 0, j = 0, k = left;
    while(i < (middle - left + 1) && j < (right - middle)){
        if(temp[i] < temp2[j]){
            A[k++] = temp[i++];
        } else{
            A[k++] = temp2[j++];
        }
    }
    while(i < (middle - left + 1)){
        A[k++] = temp[i++];
    }
    while(j < (right - middle)){
        A[k++] = temp2[j++];
    }
}

void sortSerial(int A[], int left, int right){
    if(left < right){
        int middle = (left + right) / 2;
        sortSerial(A, left, middle);
        sortSerial(A, middle + 1, right);
        merge(A, left, middle, right);
    }
}

void sort(int A[], int left, int right){
        if(left < right){
        if((right - left) > 1000){
            int middle = (left + right) / 2;
#pragma omp task firstprivate (A, left, middle)
            sort(A, left, middle);
#pragma omp task firstprivate (A, middle, right)
            sort(A, middle + 1, right);
#pragma omp taskwait
            merge(A, left, middle, right);
        } else{ sortSerial(A, left, right); }
    }
}


int main(int argc, char **argv){
    if(argc == 3){
        double begin, end;
        int n = atoi(argv[1]);
        int n_threads = atoi(argv[2]);
        omp_set_num_threads(n_threads);

        int *A = malloc(n * sizeof(int));
        int *B = malloc(n * sizeof(int));
        if(!A || !B){
            //fputs("Failed to allocate memory!\n", stderr);
            fputs("Failed to allocate memory!\n", stdout);
            return EXIT_FAILURE;
        }

#pragma omp parallel for schedule(static) default(none) shared(A, B, n)
        for(int i = 0; i < n; i++){
            *(A + i) = n - i;
        }

        //display_array(A, n);

        memcpy(B, A, n * sizeof(int));
        begin = omp_get_wtime();
        qsort(B, n, sizeof(int), compare);
        end = omp_get_wtime();
        double qsort_time = end - begin;
        printf("QSort Elapsed Time: %f s.\n", qsort_time);
        //display_array(A, n);

        begin = omp_get_wtime();
#pragma omp parallel
        {
#pragma omp single
            {
                printf("Total threads: %d\n", omp_get_num_threads());
                sort(A, 0, n - 1);
            }
        }

        //display_array(A, n);
        end = omp_get_wtime();
        double omp_time = end - begin;
        printf("OMP MergeSort Elapsed Time: %f s.\n", omp_time);
        if(omp_time > qsort_time){
            //POLUS outputs stderr in .err file
            //fputs("TEST FAILED... Qsort > OMP MergeSort\n", stderr);
            fputs("TEST FAILED... Qsort > OMP MergeSort\n", stdout);
            goto FAIL;
        }

        if(memcmp(A, B, n) != 0){
            //fputs("TEST FAILED... Arrays are not same.\n", stdout);
            fputs("TEST FAILED... Arrays are not same.\n", stdout);
            goto FAIL;
        }

        fputs("TEST PASSED!...\n", stdout);

        FAIL:
        free(A);
        free(B);
    } else{
        //fputs("Command line format:\n", stderr);
        fputs("Command line format:\n", stdout);
        //fputs("./<binary> <array_size> <number_of_open_mp_threads>\n", stderr);
        fputs("./<binary> <array_size> <number_of_open_mp_threads>\n", stdout);
        fputs("For example: ./SortOMP 100500 4\n", stdout);
    }

    return 0;
}
