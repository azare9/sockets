#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
/*
*converting a human readable number written in hexadicimal 0x1234 into
*a readable hexadicimale readable by the machine ex: man can read on hex 0x1245
*and the machine read it as : 0x 4512 so to make machine read like human 0x1245 we apply
*htons for port number and htonl for ipadress
*/
struct mv{
	int l,v;
	float c;
	char sin[8];
};

int main(int argc, char *argv[])
{
	struct mv issa;
	memset(&issa,0,sizeof(issa));
	
	unsigned short host_port=0x1234;
	unsigned short net_port;
	unsigned long host_addr=0x12345678;
	unsigned long net_addr;
	
	net_port=htons(host_port);
	net_addr=htonl(host_addr);
	
	printf("host ordered port: %#x \n", host_port);
	printf("network ordered port: %#x \n", net_port);
	printf("host ordered address: %#x \n", host_addr);
	printf("network ordered address: %#x \n", net_addr);
	
	printf("\n_________________\n");
	printf("l =: %d et v = %d \n", issa.l,issa.v);
	printf("c =: %f \n", issa.c);
	for(int i=0;i<8;i++)
	{
		printf("%d ", issa.sin[i]);
	}
	printf("\n");
	return 0;
}
