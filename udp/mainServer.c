#include "server.h"

int main(int argc, char **argv) { 
	int len_buff;
	char buffer[MAXLINE]; 

	if (argc < 1){
		printf("Usage: %s port", argv[0]);
		return -1;
	}
	else
		len_buff = udp_server(argv[1], buffer);

	if(len_buff == 0) 
		printf("Nothing in the buffer\n");
	else	
		printf("-> Client : %s\n", buffer); 

	return 0; 
} 