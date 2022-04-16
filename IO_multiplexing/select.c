#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	fd_set fds, copy_fds;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;
	
	FD_ZERO(&fds);
	FD_SET(0, &fds);//0 is standard input(console)
	
	/*
	timeout.tv_sec=5;
	timeout.tv_usec=50000;
	*/
	
	while(1)
	{
		copy_fds = fds;
		timeout.tv_sec=5;
		timeout.tv_usec=50000;
		//if there is no change during timeout than don't hang the instruction inside select longer than timeout
		//the select system call tells whether there is any data to be read on the file descriptors of interest (that one specified with FD_SET macro)
		result = select(1, &copy_fds, 0 , 0, &timeout);
		if(result == -1)
		{
			puts("select() error");
			break;
		}else if(result ==0)
		{
			puts("time-out");
		}else {
			if(FD_ISSET(0, &copy_fds))
			{
			str_len=read(0,buf, BUF_SIZE);
			buf[str_len] = 0;
			printf("message from console: %s", buf);
			}
		
		}
		
	}
	return 0;

}
