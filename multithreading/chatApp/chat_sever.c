#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h> 
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <pthread.h>

//gcc chat_sever.c -o server -lpthread -D_REENTRANT
//./server port_name

/*send back the message that has been sent by client
*/
#define BUF_SIZE 100
#define MAX_CLNT 256
void error_handling(char *message);
void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void send_msg_cible(char *msg, int len, int indice_fd);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
//names of the client connected
int names_cnt=0;
char names[MAX_CLNT][21];
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
		//read the message of connection of the client and put it it into msg

	        pthread_mutex_lock(&mutex);
	        clnt_socks[clnt_cnt++]=clnt_sock;	 
	        pthread_mutex_unlock(&mutex); 
	        
	        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
	        //free the ressources of threads
	        pthread_detach(t_id);
	        
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
	int str_len=0, i,count_oct=0;
	char msg[BUF_SIZE];
	char msgcpy[BUF_SIZE];
	memset(msg,'\0' , BUF_SIZE);
	memset(msgcpy,'\0' , BUF_SIZE);
	//name of the client that has sent the msg
	char name[20];
	bool clnt_trouve=false;
	
	read(clnt_sock,name,sizeof(msg));	        		
	printf("connected client : %s len %ld with fd %d \n",name,strlen(name),clnt_sock);
	pthread_mutex_lock(&mutex);
	strcpy(names[names_cnt++], name); 
	pthread_mutex_unlock(&mutex); 
	strcpy(msgcpy,name);
	count_oct = strlen(name);
	//printf("the first msg %s\n",msgcpy);
	//int indice_nameClnt;

	while((str_len=read(clnt_sock,msg,BUF_SIZE-1))!=0){
		memset(msgcpy,'\0' , BUF_SIZE);
		strcpy(msgcpy,name);
		//printf("le message en msgcpyest %s ",msgcpy);
		//memset(msgcpy+count_oct,'\0' , BUF_SIZE);
		/*extraire le nom du cibler du message envoyer au serveur*/
			char * pname;
			char *cible_name;
	    		pname = strtok (msg," ");
	    		cible_name = pname;
	    		int j=count_oct;	
	    		while(pname!=NULL)
	    		{	    		   
	    		   //printf("%s\n", pname);
	    		   //recontruction du msg on mettant le nom du emetteur suivie du msg que nous voulons envoyer
	    		   pname = strtok (NULL," ");
	    		   if(pname!=NULL)
	    		   {
		    		   msgcpy[j]=' ';
		    		   j++;	
		    		   strcpy(msgcpy+j,pname);
		    		   j+=strlen(pname);	    		   	    		   
	    		   }
	    		} 
	    		 
	    		//printf("msg vres la cible %s",msgcpy);
	    		/*la recherche du fd correspond au nom de la cible pname*/
	    		int k;
	    		for(k=0;k<names_cnt;k++)
	    		{
	    		   if(strncmp(names[k] , cible_name,strlen(cible_name))==0){
			      clnt_trouve = true;
	    		      send_msg_cible(msgcpy, str_len, k);
	    		      break;
	    		   }else{
	    		      clnt_trouve = false;
	    		   }
	    		}
	    		/*si l'emeteur envoie le msg a une personne qui n'a pas ete connecter */
	    		if(!clnt_trouve ){
	    			for(int n=0;n<clnt_cnt ;n++)
		    		{
		    		    if( clnt_socks[n] == clnt_sock)
		    		    {
		    		       send_msg_cible("le client n'est pas trouver\n", 100, n);
		    		    }
		    		}
	    		}	  
	    		
	}
	printf("%s has deconnected \n",name);
	pthread_mutex_lock(&mutex);
	for(i=0; i<clnt_cnt ;i++)
	{
		if(clnt_sock == clnt_socks[i])
		{
		   while (i < clnt_cnt -1)
		   { 
		      clnt_socks[i] = clnt_socks[i+1];
		      memset(names[i],'\0' , 21);
		      strcpy(names[i], names[i+1]);
		      i++;
		   }
		   break;
		}
	}	
	clnt_cnt--;
	names_cnt--;
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

void send_msg_cible(char *msg, int len, int indice_fd){// send to cible
	
	int i;
	pthread_mutex_lock(&mutex);
	    write(clnt_socks[indice_fd], msg, len);
	pthread_mutex_unlock(&mutex);    
}

