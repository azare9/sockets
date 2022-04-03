#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>
#include <sys/wait.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);
void read_childproc(int sig);
/*client can receive data from server and send data to the server at the same time
*/

int main(int argc, char *argv[])
{
	//variables
	int sock, state;
	pid_t pid;
	char buf[BUF_SIZE];
	//int str_len;
	struct sockaddr_in serv_adr;
	//signal 
	struct sigaction act;
	
	if(argc!=3){
		printf("usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	
	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	//os i want to register to SIGCHILD event with this read_childproc handler 
	state=sigaction(SIGCHLD, &act, 0);
	
	//socket creation
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		printf("failed to create a socket\n");
		exit(1);//exception
	}
	
	//connect
	bzero(&serv_adr, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr =inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	
	if(connect(sock , (struct sockaddr *)&serv_adr,sizeof(serv_adr)) ==-1){
		printf("failed to connect\n");
		exit(1);
	}
	
	//if we are in the child process we write to the server
	pid = fork();
	if(pid==0)
		write_routine(sock, buf);
	//we read from server	
	else
		read_routine(sock, buf);
	
	
	//close
	close(sock);
	printf("socket closed\n");
	return 0;
}

void read_routine(int sock, char *buf)
{
	while(1)
	{
		int str_len=read(sock, buf, BUF_SIZE-1);
		if(str_len==0)
			return;
			
		buf[str_len]=0;
		printf("Message from server: %s", buf);
	}
}
void write_routine(int sock, char *buf)
{
	while(1)
	{
		fgets(buf,BUF_SIZE, stdin);
		
		if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1 , &status , WNOHANG);
	printf("removed proc id : %d \n", pid);
}


