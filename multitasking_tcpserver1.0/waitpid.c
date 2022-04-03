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
		sleep(15);
		return 24;
	}
	else
	{	//waiting for any child to terminates
		//WNOHANG flag mean : it will not suspends execution of the body of while
		//loop the system will keep running the while each 3 sec and if child haas been terminated the while will break
		while(!waitpid(-1,&status,WNOHANG))
		{
			sleep(3);
			puts("sleep 3 sec\n");
		}
		
			if(WIFEXITED(status))
				printf("child send one: %d\n",WIFEXITED(status)); 
		
	}
		
	return 0;
}
