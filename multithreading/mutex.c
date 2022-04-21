#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define NUM_THREAD 10

// gcc mutex.c -o mutex -lpthread -D_REENTRANT
void *thread_inc(void *arg);
void *thread_dec(void *arg);
long long num=0;
pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;
	pthread_mutex_init(&mutex, NULL);
	
	printf("sizeof long long: %d \n",sizeof(long long));
	for(i=0; i< NUM_THREAD ;i++)
	{
		if(i%2)
		    pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
		    pthread_create(&(thread_id[i]), NULL, thread_dec, NULL);
	}
	
	for(i=0; i< NUM_THREAD ;i++)
	{
	     pthread_join(thread_id[i], NULL);
	}
	
	printf("result: %lld \n",num);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *thread_inc(void *arg){
	pthread_mutex_lock(&mutex);
	for(int i=0;i<50000000;i++)
		num+=1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}
void *thread_dec(void *arg){
	pthread_mutex_lock(&mutex);
	for(int i=0;i<50000000;i++)
		num-=1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}


