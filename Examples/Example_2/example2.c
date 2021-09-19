#include <pthread.h>
#include <stdio.h>
#include <errno.h>

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
#define NUM_THREADS 6

int g_value = 0;
void *inc_value(void *arg)
{
    static const int MAX_ITERATIONS = 10000;
    for (int i = 0; i < MAX_ITERATIONS; ++i)
    {
        //pthread_mutex_lock(&g_mutex);
        int err = pthread_mutex_trylock(&g_mutex);
        int tmp = g_value;
        tmp++;
        g_value = tmp;
        if (err != EBUSY)
        {
            pthread_mutex_unlock(&g_mutex);
        }
    }
    return NULL;
}

void mutex_run()
{
    pthread_t tids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(tids + i, NULL, inc_value, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        void *ret_val = NULL;
        pthread_join(tids[i], &ret_val);
    }
    printf("g_value == %d\n", g_value);
}

int main()
{
    mutex_run();
    return 0;
}