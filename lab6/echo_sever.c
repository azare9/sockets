#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

/*send back the message that has been sent by client
*/
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	//variables
	int serv_Sock, clnt_Socket;
	char message[BUF_SIZE];
	int str_len, i;
	struct sockaddr_in serverAddress,clientAddress;
	socklen_t clnt_adr_sz;
	
	if(argc !=2){
		printf("Usage :%s <port>\n", argv[0]);
		exit(1);
	}
	
	//socket creation
	serv_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_Sock == -1){
		printf("failed to create a socket\n");
		exit(1);//exception
	}else{
		printf("socket : %d \n",serv_Sock);
	}
	
	//bind the serverSocket File descriptor to the server address
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(atoi(argv[1]));
	if(bind(serv_Sock,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) ==-1){
		printf("failed to bind\n");
		exit(1);
	}else{
		printf("bound\n");
	}
	
	//listening the client connection we have 5 connection in backlog(queue)
	if(listen(serv_Sock,5) ==-1){
		printf("failed to listen\n");
		exit(1);
	}else{
		printf("listening....\n");
	}
	//accepting
	bzero(&clientAddress, sizeof(clientAddress));
	clnt_adr_sz = sizeof(clientAddress);
	
	for(i=0; i<5 ; i++)
	{
		clnt_Socket = accept(serv_Sock,(struct sockaddr*)&clientAddress, &clnt_adr_sz);
		if(clnt_Socket == -1){
			printf("failed to accepting the client connection request\n");
			exit(1);
		}else{
			printf("connected client %d \n",i+1);
		}
		while((str_len=read(clnt_Socket,message,BUF_SIZE))!=0)
			write(clnt_Socket,message,str_len);
			
		close(clnt_Socket);	
	}
	
	close(serv_Sock);
	printf("server socket closed\n");
	
	return 0;
}

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}


