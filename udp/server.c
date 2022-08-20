#include "server.h"

int udp_server(char* port, char* buffer){
	int PORT = atoi(port);
	int sockfd; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr,  0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	socklen_t len;
	int n; 

	len = sizeof(cliaddr); 

	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, 
				 &len); 
	buffer[n] = '\0';

	sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &cliaddr, 
			len); 
	//printf("<- Hello message sent.\n"); 

	return n;
}
