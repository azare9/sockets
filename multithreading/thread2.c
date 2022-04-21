#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

// gcc thread2.c-o tr2 -lpthread -D_REENTRANT

void* thread_main(void *arg)
{
	int i;
	int cnt=*((int*)arg);
	char *msg= (char *) malloc(sizeof(char)* 50);
	strcpy(msg , "Hello ,i'am thread \n");
	for(i=0; i<cnt ;i++)
	{
		sleep(1);puts("running thread");
	}
	return (void*) msg;
}

int main(int argc, char *argv[])
{
	pthread_t t_id;
	int thread_param=5;
	void *thr_ret;

	if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0)
	{
		puts("pthread_create() error");
		return -1;
	}
	//value return by the thread of t_id is stored in thr_ret
	if(pthread_join(t_id, &thr_ret)!=0)
	{
		puts("pthread_join() error");
		return -1;
	}
	printf("thread return message: %s \n",(char*)thr_ret);
	free(thr_ret);
	return 0;
}



