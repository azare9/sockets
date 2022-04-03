#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out\n");
	//os weak me up after two second 
	alarm(2);
}

int main(int argc , char *argv[])
{
	int i;
	struct sigaction act;
	act.sa_handler=timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGALRM, &act, 0);
	
	alarm(2);
	for(i=0;i<3;i++)
	{
		puts("wait ...");
		sleep(12);
	}	
		
	return 0;
}
