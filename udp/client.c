#include "client.h"

int udp_client(char* port, char* destName, unsigned char* msg, size_t size_msg, char* buffer_answer){
	int sock; 
	struct sockaddr_in servaddr; 
	struct addrinfo af_hints, *af_result = NULL;
	memset(&af_hints, 0, sizeof(struct addrinfo));
    af_hints.ai_family   = AF_INET;    /* IPv4 */
    af_hints.ai_socktype = SOCK_DGRAM; /* UDP */

	int r = -1; /* c'est la valeur que la fonction doit renvoyer. Si r = -1 alors une erreur s'est produit. Sinon r = longeur du buffer_answer   */

	int err = getaddrinfo(destName, port, &af_hints, &af_result);
	if (err) {
      fprintf(stderr,"[erreur] - getaddinfo(\"%s\") -> \"%s\"\n", destName, gai_strerror(err));
      return EXIT_FAILURE;
    }
	// Creating socket file descriptor 
	if ( (sock = socket(af_result->ai_family , af_result->ai_socktype , af_result->ai_protocol)) < 0 ) { 
		perror("socket creation failed"); 
		return -1;
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	ssize_t len;
    if ( (len = sendto(sock, msg, size_msg, 0, af_result->ai_addr, sizeof(struct sockaddr_in))) < 0) {
      perror("[erreur] - sendto ");
      return -1;
    }

	/* Récéption du datagram par le serveur DNS*/
	struct sockaddr_in addrRemote; 
    socklen_t addrRemotelen = 0;

    if ( (len = recvfrom(sock, buffer_answer, MAXLINE, 0, (struct sockaddr *) &addrRemote, &addrRemotelen)) < 0) {
      perror("[erreur] - recvfrom ");
      return -1;
    }
	
	r = len;

	printf("\n");
	/* 6) affichage de la reponse */
    

    /* fermeture du socket */
    close(sock);

	return r;
}