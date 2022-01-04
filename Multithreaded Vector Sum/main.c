/*
restrict - это ключевое слово в Си, когда используется, 
если например int *restrict x, компилятор понимает что только эта указатель ссылается на эту область в памяти 
и другие указатели не могут указывать на нее 
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int num_threads;
int *vec;
int vec_length = 0;
int vec_sum_index = 0;
int *sum_vec;
int t_index = 0;

int vector_sum(int *v, int size, int nthreads)
{
    int p = t_index++;
    for (int i = p * (size / num_threads); i < (p + 1) * (size / num_threads); i++)
    {
        sum_vec[p] += vec[i];
    }

    return 0;
}
void *vec_sum(void *arg)
{
    vector_sum(vec, vec_length, num_threads);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        int i, total_sum = 0;
        num_threads = atoi(argv[2]);
        vec_length = atoi(argv[1]);
        vec = malloc(vec_length * sizeof(int));
        for (int i = 0; i < vec_length; i++)
        {
            *(vec + i) = i + 1;
        }

        for (int i = 0; i < vec_length; i++)
        {
            printf("%d ", vec[i]);
        }
        printf("\n");

        if (num_threads > 64)
        {
            fprintf(stderr, "Maximum 64 threads can be used!");
        }
        pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
        if (threads == NULL)
        {
            fprintf(stderr, "Error allocating memory for threads!");
            return 2;
        }
        int *threadID = malloc(num_threads * sizeof(int));
        if (threadID == NULL)
        {
            fprintf(stderr, "Error allocating memory for threadID!");
            return 3;
        }

        sum_vec = malloc(num_threads * sizeof(int));
        if (sum_vec == NULL)
        {
            fprintf(stderr, "Error allocating memory for sum!");
            return 4;
        }

        for (int i = 0; i < num_threads; i++)
        {
            *(sum_vec + i) = 0;
        }

        for (i = 0; i < num_threads; i++)
        {
            threadID[i] = i;
            pthread_create(&threads[i], NULL, vec_sum, threadID + i);
        }

        for (i = 0; i < num_threads; i++)
        {
            pthread_join(threads[i], NULL);
        }

        for (int i = 0; i < num_threads; i++)
            total_sum += sum_vec[i];

        printf("Sum = %d\n", total_sum);

        free(threadID);
        free(threads);
        free(vec);
        free(sum_vec);
    }
    else
    {
        printf("Command line format\n ./<binary> <vector-length> <num-threads>\n");
        return 5;
    }

    return 0;
}