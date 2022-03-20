#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	//variables
	int sock;
	char msg1[]="Hi!";
	char msg2[]="I'm another UDP host";
	char msg3[]="Nice to meet you";

	socklen_t your_adr_sz;
	struct sockaddr_in your_adr;
	
	if(argc!=3){
		printf("usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	//UDP socket creation
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == -1){
		printf("failed to create a socket\n");
		exit(1);//exception
	}else{
		printf("socket : %d \n",sock);
	}
	
	//connect
	memset(&your_adr, 0,sizeof(your_adr));
	your_adr.sin_family = AF_INET;
	your_adr.sin_addr.s_addr =inet_addr(argv[1]);
	your_adr.sin_port = htons(atoi(argv[2]));
	
	
	sendto(sock,msg1,sizeof(msg1),0,(struct sockaddr*)&your_adr,sizeof(your_adr));
	
	sendto(sock,msg2,sizeof(msg2),0,(struct sockaddr*)&your_adr,sizeof(your_adr));
	
	sendto(sock,msg3,sizeof(msg3),0,(struct sockaddr*)&your_adr,sizeof(your_adr));
	//close
	close(sock);
	printf("socket closed\n");
	return 0;
}
