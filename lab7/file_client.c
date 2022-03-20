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
	int sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;
	if(argc!=3){
		printf("Usage: %s <port>\n",argv[0]);
		exit(1);
	}
	fp = fopen("receive.dat","wb");
	
	//socket creation
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd == -1){
		printf("failed to create a socket\n");
		exit(1);//exception
	}else{
		printf("socket : %d \n",sd);
	}
	
	//connect
	memset(&serv_adr, 0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr =inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	if(connect(sd , (struct sockaddr *)&serv_adr,sizeof(serv_adr)) !=0){
		printf("failed to connect\n");
		exit(1);
	}else{
		printf("connected \n");
	}
	
	
	while((read_cnt=read(sd, buf, BUF_SIZE))!=0){
		fwrite((void *)buf, 1,read_cnt, fp);
	}
	
	puts("Receive file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
	printf("socket closed\n");
	
	return 0;
}
