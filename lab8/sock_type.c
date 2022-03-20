#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
	//variables
	int tcp_sock,udp_sock;
	int sock_type;
	socklen_t optlen;
	int state;
	
	optlen = sizeof(sock_type);
	tcp_sock=socket(PF_INET, SOCK_STREAM,0);
	udp_sock=socket(PF_INET, SOCK_DGRAM,0);
	printf("SOCK_STREAM: %d\n",SOCK_STREAM);
	printf("SOCK_DGRAM: %d\n",SOCK_DGRAM);
	
	state=getsockopt(tcp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
	if(state)
		error_handling("getsockopt() error!");
	printf("socket type one: %d\n",sock_type);
	
	state=getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
	if(state)
		error_handling("getsockopt() error!");
	printf("socket type one: %d\n",sock_type);

	
	return 0;
}

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

