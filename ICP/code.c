#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void read_status(){
	// read zombie childs status code
	int status;
	pid_t ch = waitpid(-1, &status ,WNOHANG);
	if(WIFEXITED(status))
		printf("child process (pid: %d) terminated with status code :%d\n",ch,WIFEXITED(status));
}
void handle_child_process(int *fds, int exitCode){
	//read message from fds
	// when '.' in the message, terminate with exitCode
	char c;
	while(read(fds[0] , &c, 1) > 0 && c != '.') 
		printf("%c",c);
	printf("\n");
	exit(exitCode);
}

int main(){
	struct sigaction sa;
	sa.sa_handler = read_status;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGCHLD , &sa ,0);
	
	// parent process scope
	//create 1st child process + IPC1
	int fds1[2];
	pipe(fds1);
	pid_t ch1 =fork();
	if(ch1 == 0){
		// child1 process scope
		handle_child_process(fds1,32);
	} else {
		// parent process scope
		//create 2nd child process + IPC2
		int fds2[2];
		pipe(fds2);
		pid_t ch2 =fork();
		if(ch2 == 0){
		// child2 process scope
		handle_child_process(fds2,23);
		} else {
		// parent process scope
		// send messages to children
		char* msg = "Hi to child1.";
		write(fds1[1] , msg , strlen(msg));
		
		msg = "how are you to chil2.";
		write(fds2[1] , msg , strlen(msg));
		
		//zombie
		sleep(5);
		exit(0);
	  	}
	}

}
