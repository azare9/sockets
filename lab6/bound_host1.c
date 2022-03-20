#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

/*reciving 3 message consecutif from the host2 and read them one by one with a delay of 5
*/
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	//variables
	int sock;
	char message[BUF_SIZE];
	int str_len,i;
	struct sockaddr_in my_adr,your_adr;
	
	
	if(argc !=2){
		printf("Usage :%s <port>\n", argv[0]);
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
	
	//bind the serverSocket File descriptor to the server address
	memset(&my_adr, 0 ,sizeof(my_adr));
	my_adr.sin_family = AF_INET;
	my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_adr.sin_port = htons(atoi(argv[1]));
	if(bind(sock,(struct sockaddr *)&my_adr,sizeof(my_adr)) ==-1){
		printf("failed to bind\n");
		exit(1);
	}else{
		printf("bound\n");
	}
		
	for(i=0; i<3 ; i++)
	{
		sleep(5);// delay 5 sec
		int adr_sz = sizeof(your_adr);
		/*udp recive and send one message at a time so if you make recvfrom function call even though all the byte(msg1,msg2,msg3 from host 2) has been tranfered to host1 each time you call recivefrom you will recive the result of a single sent of function sendto of host1 but in TCP its not like that you will recive all the messages one after the other
		*/
		str_len = recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&your_adr, &adr_sz);
		printf("Message %d: %s\n",i+1, message);	  
	}
	
	close(sock);
	printf("server socket closed\n");
	
	return 0;
}

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}


