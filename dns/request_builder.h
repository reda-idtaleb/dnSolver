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

#ifndef _question_H_
#define _question_H_

#define HEADER     12     /*                                 */
#define QTYPE       2     /* Longueur des différents champs  */        
#define QCLASS      2     /*                                 */

size_t len_question;

/* Définition de l'entete de la requete */
extern void define_header_question_DNSRequest(unsigned char *query);

/* conversion de la question */
extern void convert_question_DNSFormat(const char *question, unsigned char *query);

/* définition du champs Qtype */
extern void define_qtype_question_DNSRequest(unsigned char* query);

/* définition du champs Qclass */
extern void define_qclass_question_DNSRequest(unsigned char* query);

/* fonction principale: rempli les champs de la requête */
extern void create_DNS_request(const char *question, unsigned char *query, size_t len_request);

#endif

