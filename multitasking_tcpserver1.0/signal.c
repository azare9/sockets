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
void keycontrol(int sig)
{
	if(sig==SIGINT)
		puts("CTRL +C pressed\n");
}
int main(int argc , char *argv[])
{
	int i;
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);
	for(i=0;i<3;i++)
	{
		puts("wait ...");
		sleep(12);
	}	
		
	return 0;
}
