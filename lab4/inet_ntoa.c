#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

/*
*converting the network ordered ip adress in hex 0x4f7ce87f readable by machine into 
*human readable ip adress "127.232.124.79"
*/

int main(int argc, char *argv[])
{
	struct sockaddr_in addr1, addr2;
	char *str_ptr;
	//char str_arr[20];
	
	addr1.sin_addr.s_addr=htonl(0x10203040);
	addr2.sin_addr.s_addr=htonl(0x10101010);
	
	str_ptr=inet_ntoa(addr1.sin_addr);
	//strcpy(str_arr,str_ptr);
	printf("Dotted-Decimal: %s \n", str_ptr);
	
	
	inet_ntoa(addr2.sin_addr);
	printf("Dotted-Decimal notation2: %s \n", str_ptr);
	printf("_______________\n");
	
	return 0;
}



