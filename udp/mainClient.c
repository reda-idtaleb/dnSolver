#include "client.h"

int main(int argc, char **argv) { 
	char buffer[MAXLINE]; 
	int len_answer = -1;

	char *msg = argv[2];

	if(argc < 2){
		printf("USAGE: %s port message\n", argv[0]);
		return -1;
	}	
	else
		len_answer = udp_client(argv[1], argv[2], (unsigned char *)msg, strlen(msg)+2, buffer);

	if (len_answer == -1)
		printf("Erreur dans l'envoie du packet UDP");
	else
		printf("Answer: %s\n", buffer);
	
	return 0; 
} 
