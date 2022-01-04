#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

double result = 0.0;         // the result
int num_partition_intervals; // the number of intervals from 0 to 1
int num_threads;             // number of threads
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

double f(double a)
{
    return (4.0 / (1 + a * a));
}

void *EstimatePi(void *arg)
{
    double x, width, tmp = 0;
    int i, threadID = *((int *)arg);

    width = 1.0 / num_partition_intervals;

    for (i = threadID; i < num_partition_intervals; i += num_threads)
    {
        x = ((double)i + 0.5) * width;
        tmp += f(x);
    }

    //tmp *= width;

    pthread_mutex_lock(&mut);
    result = result + width * tmp;
    pthread_mutex_unlock(&mut);
    return NULL;
}

int main(int argc, char **argv)
{
    clock_t begin = clock();
    if (argc == 3)
    {
        int i;
        num_partition_intervals = atoi(argv[1]);
        num_threads = atoi(argv[2]);
        pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
        if (threads == NULL)
        {
            fprintf(stderr, "Error allocating memory for threads!");
            return 1;
        }
        int *threadID = malloc(num_threads * sizeof(int));
        if (threadID == NULL)
        {
            fprintf(stderr, "Error allocating memory for threadID!");
            return 2;
        }

        for (i = 0; i < num_threads; i++)
        {
            threadID[i] = i;
            pthread_create(&threads[i], NULL, EstimatePi, threadID + i);
        }

        for (i = 0; i < num_threads; i++)
        {
            pthread_join(threads[i], NULL);
        }
        clock_t end = clock();
        printf("%f \n", result);
        double elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Elapsed time: %f s\n", elapsed_time);
        free(threads);
        free(threadID);
    }
    else
    {
        printf("Command line format\n ./<binary> <num-partition-intervals> <num-threads>\n");
    }

    return 0;
}
