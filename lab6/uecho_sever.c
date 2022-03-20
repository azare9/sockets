#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

/*send back the message that has been sent by client
*/
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	//variables
	int serv_Sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serverAddress,clientAddress;
	socklen_t clnt_adr_sz;
	
	if(argc !=2){
		printf("Usage :%s <port>\n", argv[0]);
		exit(1);
	}
	
	//UDP socket creation
	serv_Sock = socket(AF_INET, SOCK_DGRAM, 0);
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
		
	while(1)
	{
		clnt_adr_sz = sizeof(clientAddress);
		str_len = recvfrom(serv_Sock,message,BUF_SIZE,0,(struct sockaddr*)&clientAddress, &clnt_adr_sz);
			  sendto(serv_Sock,message,str_len,0,(struct sockaddr*)&clientAddress,clnt_adr_sz);
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


