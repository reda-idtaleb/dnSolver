#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <math.h>  

#define MAX_TAILLE 512

/* extraction du opcode(4 bits) du champ parametres de l'entête */
extern char* decode_opcode(unsigned char *answer);

/* extraction du RCODE (last 4 bit) du champ parametres de l'entête */
extern char* decode_rcode(unsigned char *answer);

/* extraction des flags (QR, AA, TC, RD, RA) du champ parameters de l'entête 
   Remplit le tableau res_flags par les flags*/
extern void decode_flags_AA_TC_RD_RA(unsigned char *answer, char *res_flags);

/*  */
extern void decode_question(unsigned char *answer);

/* Affichage de tous les champs de la réponse fournit par le serveur */
extern void display_DNS_respons(unsigned char *answer, int answer_lentgh);

/* Affichage de la question de la section question */
extern void display_question_section(unsigned char *answer);

/* extraction du QType de la requête */
extern char* decode_Qtype(unsigned char* answer, int offset);

/* affichage du QClass de la requête */
extern char* decode_Qclass();

/* permet de décoder les pointeurs récursivement 
  
   param answer: le tableau original contenant la réponse fournit par le DNS
   param res   : le tableau résultat contenant la chaine décodé
   param pos   : l'offset où pointe le pointeur lu
   param debut : toujours à 0; 
   
   return      : la longeur de tout le pointeur décodé 
*/ 
extern int recursive_searching(unsigned char *answer, unsigned char *res, int pos, unsigned int debut);

/* permet de construire un tableau contenant les champs de réponses décodé(sans pointeurs)
   renvoie la longueur du tableau RES 
*/ 
extern int develop_answer(unsigned char *answer, int answer_lentgh, unsigned char *res);

/* affichage la section réponse*/
extern void display_answer_response(unsigned char *answer, int answer_lentgh);

/* convertir le format url(3www4lifl2fr) en string(www.lifl.fr.) 
   Le résultat est stocké dans le tableau URL_FORMATTED
*/
extern void format_url(unsigned char* url, char *url_formatted);

/* Affichage des lignes de chaque réponse 

   param DEVELOPED_ANSWER: le tableau contenant la réponse décodé
   param LENTGH_DEV_ANSWER: la longueur du tableau réponse 
   param OFFSET: la position de la prochaine réponse à lire (toujours à 0)

   return: renvoie la position de la prochaine réponse
*/
extern int display_one_answer(unsigned char* developed_answer, int lentgh_dev_answer, int offset);
