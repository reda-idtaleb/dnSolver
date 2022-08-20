#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include "request_builder.h"
#include "../udp/client.h"
#include "response_analyser.h"

#define DESTNAME "1.1.1.1" 
/* PORT du serveur DNS qui recevra notre requete */
#define DESTPORT "53"

#define NS_ANSWER_MAXLEN 512

/* On vérifie si l'ARGUMENT ne contient pas de caractère non imprimables ou non prise en charge
   Renvoi 0 si l'argument est valide, -1 sinon 
*/
int verify_argument(char* argument, int arg_len){
    int i;
    int is_valid = -1;
    const char c = ' ';
    for(i = 0; i < arg_len; i++){   
        if (argument[i] >= 33 && argument[i] <= 126)
            is_valid = 0;
        else{   
            is_valid = -1;
            break;
        }        
    }
    return is_valid;
}

int main(int argc, char** argv) {
    const char* question;
    size_t len_request;
    char buffer_answer[NS_ANSWER_MAXLEN];
    int is_valid_argument;

    /* vérification des arguments */ 		
    if(argc < 2){
        question = NULL;
        is_valid_argument = -1;
        printf("Program Failed!\nUsage: %s URLFormat\nExample of URL: www.exemple.domainName\n", argv[0]); 
        return -1;
    }    
    else{
        question = argv[1];
        /* verifier l'argument saisi par l'utilisateur */
        is_valid_argument = verify_argument((char *)question, strlen(question));
        if (is_valid_argument == 0)    
            len_question = strlen(question) + 2; /* 2 octets ajoutés(1er octet est le dernier element('r') et 2ème est le \0 fin de tableau) */       
        else{
            printf(";  <<>> Warning <<>>\n;; The format of the adress is not respected!\n;; Do not use accented characters or any other non-printable character.\n\n");
            printf(";; Only: *  a-z\n");
            printf(";;       *  0-9\n");
            printf(";;       *  - (hyphen)\n");
            printf(";;       *  ! \" # $ & ' ( ) * + , - / : ; < = > ? @ [ \\ ] ^ _ ` { | } ~ . \n; Quit\n\n");
            
            return -1;
        }    
    }

    /* calcule de la taille du tableau query réprésentant la requête */
    len_request =  HEADER + len_question + QTYPE + QCLASS; 

    /* déclaration du buffer représentant une requête(question) */
    unsigned char query[len_request];
    /* Initialisation */
    memset(&query, 0, len_request );
    
    /* Conversion de la question en une requete DNS */
    create_DNS_request(question, query, len_request); 

    /* envoie du packet UDP au serveur DNS, remplit également le buffer_answer par la réponse fournie par le serveur DNS */	
    int len = udp_client(DESTPORT, DESTNAME, query, len_request, buffer_answer);
    
     /* Affichage du contenu du tableau */
    display_DNS_respons(buffer_answer, len);

    if (len == -1)
        printf("nothing on buffer\n"); 

    printf("\n");    
    exit(0);
}
