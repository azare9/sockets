#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

int main(int argc, char *argv[])
{
	//variables
	int socketFd;
	struct sockaddr_in serverAddress;
	
	//socket creation
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFd == -1){
		printf("failed to create a socket\n");
		exit(1);//exception
	}else{
		printf("socket : %d \n",socketFd);
	}
	
	//connect
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr =inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(4444);
	if(connect(socketFd , (struct sockaddr *)&serverAddress,sizeof(serverAddress)) !=0){
		printf("failed to connect\n");
		exit(1);
	}else{
		printf("connected \n");
	}
	
	//read:in order to read the message from server we need a memory area where we will be loading the msg that comming from tcp server
	char buf[128];//in hello word! sent by server we need only 13 byte but if we want we can increase the size of the buffer
	//1
	read(socketFd ,buf,128);
	printf("received : %s\n", buf);
	//1
	
	//echo client
	//write(socketFd,msg,strlen(msg));
	//int str_len=read(socketFd,msg,BUFSIZ-1);
	//echo client
	
	//close
	close(socketFd);
	printf("socket closed\n");
	return 0;
}
