#include <stdio.h>
#include <unistd.h>

int main(int argc , char *argv[])
{
	pid_t pid = fork();
	
	if(pid==0)
	{
		puts("Hi i'am a child process");
	}
	else
	{
		printf("child process ID: %d \n",pid);
		sleep(10);
	}
	
	if(pid==0)
		puts("End child process\n");
	else
		puts("End parent processs\n");
		
	return 0;
}
