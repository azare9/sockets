#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

/*send back the message that has been sent by client
*/
#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[])
{
	//variables
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr,clnt_adr;
	struct timeval timeout;
	fd_set fds, cpy_fds;
	
	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	
	if(argc != 2){
		printf("usage : %s <port>\n", argv[0]);
		exit(1);
	}

	//socket creation
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("server ***** socket: %d \n", serv_sock);
		
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

	//listening the client connection
	if(listen(serv_sock,1) ==-1){
		error_handling("listen error\n");
	}

	FD_ZERO(&fds);
	FD_SET(serv_sock, &fds);
	fd_max = serv_sock;
	 
	 
	while(1)
	{
		cpy_fds = fds;
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
		
		if((fd_num = select(fd_max +1, &cpy_fds, NULL, NULL, &timeout)) == -1){
			break;
		}
		//there are no change in fd_set
		if(fd_num == 0){
			continue;	
		}
		
		for(i= 0; i< fd_max +1; i++)
		{
		   if(FD_ISSET(i, &cpy_fds)) //is the i-th fd the one of our interest
		   {
		      if(i == serv_sock) //connection request
		      {
		         //accepting
			 //bzero(&clnt_adr, sizeof(clnt_adr));
			 adr_sz=sizeof(clnt_adr);
			 clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr, &adr_sz);
			 FD_SET(clnt_sock, &fds);
			 if(fd_max < clnt_sock) fd_max = clnt_sock;
			 printf("connection client: %d \n", clnt_sock);
		      }
		      else //read message
		      {
		         str_len =read(i, buf, BUF_SIZE);
		         if(str_len == 0) //close request, if the num of byte sent by the client == 0 i.e the FIN flag then close client socket
		         {
		            FD_CLR(i, &fds);
		            close(i);
		            printf("closed client: %d \n", i);		        
		         }
		         else // echo		    
		            write(i, buf , str_len);		         
		      }
		   }
		}
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

