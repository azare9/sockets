#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	//variables
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;
	
	if(argc!=3){
		printf("usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	//socket creation
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		printf("failed to create a socket\n");
		exit(1);//exception
	}else{
		printf("socket : %d \n",sock);
	}
	
	//connect
	bzero(&serv_adr, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr =inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	if(connect(sock , (struct sockaddr *)&serv_adr,sizeof(serv_adr)) ==-1){
		printf("failed to connect\n");
		exit(1);
	}else{
		printf("connected \n");
	}
	
	//read:in order to read the message from server we need a memory area where we will be loading the msg that comming from tcp server
	
	while(1)
	{
		fputs("Input message(Q to quit): ",stdout);
		fgets(message,BUF_SIZE, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;
		
		write(sock, message, strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		printf("Message from server: %s", message);
	}
	
	//close
	close(sock);
	printf("socket closed\n");
	return 0;
}
