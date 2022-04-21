#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <pthread.h>

//gcc chat_sever.c -o server -lpthread -D_REENTRANT

/*send back the message that has been sent by client
*/
#define BUF_SIZE 100
#define MAX_CLNT 256
void error_handling(char *message);
void *handle_clnt(void *arg);
void send_msg(char *msg, int len);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
	//variables
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr,clnt_adr;
	int clnt_adr_sz ;
	pthread_t t_id;
	
	if(argc != 2){
		printf("usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	pthread_mutex_init(&mutex, NULL);
	
	//socket creation
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1){
		printf("failed to create a socket\n");
		exit(2);//exception
	}
	
	//bind the serverSocket File descriptor to the server address
	memset(&serv_adr,0 , sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	if(bind(serv_sock,(struct sockaddr *)&serv_adr,sizeof(serv_adr)) ==-1){
		error_handling("bind error\n");
	}
	
	//listening the client connection we have 1 connection in backlog(queue)
	if(listen(serv_sock,1) ==-1){
		error_handling("bind error\n");
	}
	
	while(1)
	{
		//accepting
		bzero(&clnt_adr, sizeof(clnt_adr));
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		
	        pthread_mutex_lock(&mutex);
	        clnt_socks[clnt_cnt++]=clnt_sock;
	        pthread_mutex_unlock(&mutex); 
	        
	        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
	        //free the ressources of threads
	        pthread_detach(t_id);
		
		printf("connected client IP: %s\n",inet_ntoa(clnt_adr.sin_addr));
	}
	
	close(serv_sock);
	printf("server socket closed\n");
	
	return 0;
}

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
void *handle_clnt(void *arg)
{
	int clnt_sock = *((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];


	while((str_len=read(clnt_sock,msg,sizeof(msg)))!=0)
		send_msg(msg, str_len);
				
	pthread_mutex_lock(&mutex);
	for(i=0; i<clnt_cnt ;i++)
	{
		if(clnt_sock == clnt_socks[i])
		{
		   while (i < clnt_cnt -1)
		   { 
		      clnt_socks[i] = clnt_socks[i+1];
		      i++;
		   }
		   break;
		}
	}	
	clnt_cnt--;
	pthread_mutex_unlock(&mutex);	
	close(clnt_sock);
	return NULL;
}
void send_msg(char *msg, int len){// send to all
	
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0; i< clnt_cnt ; i++)
	    write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutex);    
}

