#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc , char *argv[])
{
	int status;
	pid_t pid = fork();
	
	if(pid==0)
	{
		return 3;
	}
	else
	{
		printf("child process ID: %d \n",pid);
		pid = fork();
		if(pid==0)
		{
			exit(7);	
		}
		else
		{
			printf("second child pid: %d \n",pid);
			//waiting for any child to terminate
			//the wait system call suspends execution of the calling process until one of its children terminates
			//the call wait(&status) is equivalent to waitpid(-1,&status,0)
			wait(&status);
			//WIFEXITED return non zero value if the child procees terminated normally with exit or _exit
			if(WIFEXITED(status))
				printf("child send one: %d\n",WIFEXITED(status)); 
				
			wait(&status);
			//WIFEXITED return what child return
			if(WIFEXITED(status))
				printf("child send two: %d\n",WIFEXITED(status)); 
				
			sleep(10);//sleep 10 second
		}
	}
	
		
	return 0;
}
