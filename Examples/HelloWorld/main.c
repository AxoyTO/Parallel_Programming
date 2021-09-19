#include <pthread.h>
#include <stdio.h>
#include <assert.h>

struct ctx{
	int id;
	char *msg;
};

void* hello_routine(void* arg){
	struct ctx* parg = (struct ctx*)(arg);
	printf("%d (%s): hello world!\n", parg->id, parg->msg);
	return NULL;
}

//#define NUM_THREADS 6;
static const int NUM_THREADS = 6;

void join_run(){
	char* messages[NUM_THREADS];
	messages[0] ="one";
	messages[1] ="two";
	messages[2] ="three";
	messages[3] = "four";
	messages[4] ="five";
	messages[5] ="six";
	struct ctx ctxs[NUM_THREADS];
	for(int i =0; i<NUM_THREADS; i++){
		ctxs[i].id = i;
		ctxs[i].msg = messages[i];
	}

	pthread_t threads[NUM_THREADS];
	for(int i = 0; i<NUM_THREADS; i++){
		int retVal = pthread_create(&threads[i], NULL, hello_routine, ctxs+i);
		assert(retVal == 0);
	}
	
	for(int i=0; i<NUM_THREADS;i++){
		void* status=NULL;
		pthread_join(threads[i], &status);
}
}

int main(){
	join_run();
	return 0;
}
