#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

/*
*converting a human readable ip adress from string "123.344.543.566" into
*a network oredred adress readable by the machine we apply
*inet_aton for ip adress
*/

int main(int argc, char *argv[])
{
	char *addr="127.232.124.79";
	struct sockaddr_in addr_inet;
	
	if(!inet_aton(addr,&addr_inet.sin_addr))
		error_handling("conversion error");
	else
		printf("Network ordered integer addr: %#x \n",addr_inet.sin_addr);
	printf("\n");
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message ,stderr);
	fputs('\n' ,stderr);
	exit(1);
}


