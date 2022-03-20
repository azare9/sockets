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
	int serverSocketFd, clientSocketFd;
	struct sockaddr_in serverAddress,clientAddress;
	int clientAddressLength;
	
	//socket creation
	serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocketFd == -1){
		printf("failed to create a socket\n");
		exit(1);//exception
	}else{
		printf("socket : %d \n",serverSocketFd);
	}
	
	//bind the serverSocket File descriptor to the server address
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(4444);
	if(bind(serverSocketFd,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) !=0){
		printf("failed to bind\n");
		exit(1);
	}else{
		printf("bound\n");
	}
	
	//listening 1 for one client 
	if(listen(serverSocketFd,1) !=0){
		printf("failed to listen\n");
		exit(1);
	}else{
		printf("listening....\n");
	}
	//accepting
	bzero(&clientAddress, sizeof(clientAddress));
	clientSocketFd = accept(serverSocketFd,(struct sockaddr*)&clientAddress, &clientAddressLength);
	if(clientSocketFd == -1){
		printf("failed to accepting the client connection request\n");
		exit(1);
	}else{
		printf("accepted....\n");
	}

	//writing (communicating)
	char* msg ="Hello word!";
	//1
	//write(clientSocketFd, msg, strlen(msg));
	//printf("sent : \"%s\"\n" ,msg);
	//1
	
	//echo client
	int str_len;
	//while((str_len=read(clientSocketFd,msg, BUFSIZ))!=0)
	//	write(clientSocketFd,msg,str_len);
	//echo client
	//close
	close(clientSocketFd);
	printf("client socket closed\n");
	close(serverSocketFd);
	printf("server socket closed\n");
	
	return 0;
}



