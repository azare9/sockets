#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

void error_handling(char *message);
//changing and printing internal buffer sizes

int main(int argc, char *argv[])
{
	//variables
	int sock;
	int snd_buf=1024*3,rcv_buf=1024*2,state;
	int res_snd_buf,res_rcv_buf;
	socklen_t len;
	
	sock=socket(PF_INET, SOCK_STREAM,0);
	//printf("SOCK_STREAM: %d\n",SOCK_STREAM);
	
	state=setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void*)&snd_buf,sizeof(snd_buf));
	if(state)
		error_handling("getsockopt() error!");
		
	
	state=setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void*)&rcv_buf,sizeof(rcv_buf));
	if(state)
		error_handling("getsockopt() error!");
		
	len = sizeof(res_snd_buf);
	//printf("SOCK_STREAM: %d\n",SOCK_STREAM);
	
	state=getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void*)&res_snd_buf,&len);
	if(state)
		error_handling("getsockopt() error!");
		
	
	len = sizeof(res_rcv_buf);
	state=getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void*)&res_rcv_buf,&len);
	if(state)
		error_handling("getsockopt() error!");

	printf("Input buffer size: %d\n",res_rcv_buf);
	printf("Output buffer size: %d\n",res_snd_buf);

	
	return 0;
}

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}


