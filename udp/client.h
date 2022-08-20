#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>

#define MAXLINE 512

/* Send a msg to a server(specified by its port), and fills the buffer with the server answer's
   return the lentgh of the answer if sucess, otherwise an error will be raised	
 */
extern int udp_client(char* port, char* destName, unsigned char* msg, size_t size_msg, char* buffer_answer);
