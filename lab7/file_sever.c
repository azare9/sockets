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
	int serv_sd, clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;
	
	struct sockaddr_in serv_adr,clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2){
		printf("Usage: %s <port>\n",argv[0]);
		exit(1);
	}
	
	fp = fopen("file_sever.c","rb");
	//socket creation
	serv_sd = socket(AF_INET, SOCK_STREAM, 0);
	
	//bind the serverSocket File descriptor to the server address
	memset(&serv_adr, 0 ,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sd,(struct sockaddr *)&serv_adr,sizeof(serv_adr)) !=0){
		printf("failed to bind\n");
		exit(1);
	}else{
		printf("bound\n");
	}
	
	//listening 1 for one client 
	if(listen(serv_sd,1) !=0){
		printf("failed to listen\n");
		exit(1);
	}else{
		printf("listening....\n");
	}
	//accepting
	memset(&clnt_adr, 0,sizeof(clnt_adr));
	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd,(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	if(clnt_sd == -1){
		printf("failed to accepting the client connection request\n");
		exit(1);
	}else{
		printf("accepted....\n");
	}

	//writing (communicating)
	while(1){
	//reading the content of file using fread to buf array BUF_SIZE bytes
		read_cnt=fread((void*)buf, 1,BUF_SIZE,fp);
		if(read_cnt < BUF_SIZE)
		{
			write(clnt_sd,buf, read_cnt);
			break;
		}
		write(clnt_sd,buf, read_cnt);
	}
	
	//tcp half close the server can recieve message from the client but connot send back to the client
	shutdown(clnt_sd, SHUT_WR);
	read_cnt = read(clnt_sd, buf, BUF_SIZE);
	buf[read_cnt] = '\0';
	printf("Message from client : %s\n", buf);
	//close
	fclose(fp);
	printf("client socket closed\n");
	close(clnt_sd);
	close(serv_sd);
	printf("server socket closed\n");
	
	return 0;
}



