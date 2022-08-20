#include "response_analyser.h"
#include "request_builder.h"

char* decode_opcode(unsigned char *answer){
    unsigned char parameters = answer[2]; // les 4 bits après du 1èr octet paramètres 
    char *res = "";

    /* extraction du opcode flag(4 bits, after QR flag)*/
    int OPCODE_FLAG = (parameters >> 3) & ((int) pow(2, 4) - 1);

    switch(OPCODE_FLAG){
        case 0: res = "QUERY"; break;
        case 1: res = "IQUERY"; break;
        case 2: res = "STATUS"; break;
    } 

    return res;   
}

char* decode_rcode(unsigned char *answer){
    unsigned char parameters = answer[3];
    char *res = ""; 

    /* extraction du rcode flag (les derniers 4 bits du champ 'parameters')*/
    int RCODE_FLAG = parameters & ((int) pow(2, 4) - 1); 

    switch(RCODE_FLAG){
        case 0: res = "NOERROR";  break;
        case 1: res = "FORMERR";  break;
        case 2: res = "SERVFAIL"; break;
        case 3: res = "NXDOMAIN"; break;
        case 4: res = "NOTIMP";   break;
        case 5: res = "REFUSED";  break;
    }

    return res;
}

void decode_flags_AA_TC_RD_RA(unsigned char *answer, char *res_flags){
    unsigned char parameters_first_byte;
    unsigned char parameters_second_byte;
    int QR_FLAG, FLAG_RD, FLAG_TC, FLAG_AA, FLAG_RA;

    parameters_first_byte  = answer[2];
    parameters_second_byte = answer[3];
    
    if ((QR_FLAG = (parameters_first_byte & (int)pow(2, 7)) >> 7) != 0)
        strcat(res_flags, "qr");
    if ((FLAG_RD = parameters_first_byte & 1) != 0)
        strcat(res_flags, " rd");
    if (((FLAG_TC = parameters_first_byte & 2) >> 1 ) != 0)
        strcat(res_flags, " tc");
    if (((FLAG_AA = parameters_first_byte & 4) >> 2 ) != 0) 
        strcat(res_flags, " aa");
    if (((FLAG_RA = parameters_second_byte & (int)pow(2, 7)) >> 7) != 0)
        strcat(res_flags, " ra");
    
    if (strlen(res_flags) == 0) 
        strcat(res_flags, "No_Flags");   
} 

char* decode_Qtype(unsigned char* answer, int offset){
    int QTYPE_FLAG = answer[offset] + answer[offset + 1] ;
    char *res = "";

    switch(QTYPE_FLAG){
        case  1: res = "A"    ; break;
        case  2: res = "NS"   ; break;
        case  5: res = "CNAME"; break;
        case 28: res = "AAAA" ; break;
    }

    return res;
} 

char* decode_Qclass(){
    char* QCLASS_FLAG;
    QCLASS_FLAG = "IN";

    return QCLASS_FLAG;
} 

void display_DNS_respons(unsigned char *answer, int answer_lentgh){
    int query, nb_answers, authority, additional;
    char *opcode_flag, *rcode_flag;
    char aa_tc_rd_ra_flag[15] = "";
    
    query      = answer[4] + answer[5] ;  // QDcount
    nb_answers = answer[6] + answer[7] ;  // ANcount
    authority  = answer[8] + answer[9] ;  // authority
    additional = answer[10]+ answer[11];  // additional

    opcode_flag = decode_opcode(answer);
    rcode_flag  = decode_rcode (answer);
    decode_flags_AA_TC_RD_RA(answer, aa_tc_rd_ra_flag);
    
    printf(";; <<>> MY DiG Version <<>> \n");
    
    printf(";; Got answer: \n");
    printf(";; ->>HEADER<<-");
    printf(" opcode: %s, status: %s,", opcode_flag, rcode_flag);
    printf(" id: %d%d,", answer[0], answer[1]);
    printf("\n;; flags: %s,", aa_tc_rd_ra_flag);
    printf(" QUERY: %d, ANSWER: %d AUTHORITY: %d, ADDITIONAL: %d", query, nb_answers, authority, additional);
    
    display_question_section(answer);
    display_answer_response(answer, answer_lentgh); 
}

void decode_question(unsigned char *answer){
    int i, j, k, off; 
    unsigned char question[len_question-1];
    unsigned char r[2];
    off = 0;

    for(i = HEADER; answer[i] != '\0'; i++){
        if((answer[i] < 32)  ){
            for(j = 0; j < answer[i]; j++){
                question[off] = answer[j+i+1];
                off++;   
            }
            question[off] = '.';
            off++;        
        } 
    }

    for(i = 0; i < sizeof(question); i++){
        printf("%c", question[i]);
    }
        
}

void display_question_section(unsigned char *answer){
    char *qtype, *qclass;
    
    qtype  = decode_Qtype(answer, HEADER + len_question); 
    qclass = decode_Qclass();

    printf("\n\n;; QUESTION SECTION:\n; ");
    decode_question(answer);

    printf("\t\t %s\t%s\n", qtype, qclass);   
}

int recursive_searching(unsigned char *answer, unsigned char *res, int pos, unsigned int debut){
    int index = pos;
    while (answer[index] != 0x00){
        if(answer[index] == 0xc0){          
            pos = answer[index+1];             
            debut = recursive_searching(answer, res, pos, debut);
            return debut;
        }  
        else{
            res[debut] = answer[index];
            debut++;
            index ++;    
        }  
    }
    return debut;
}

int develop_answer(unsigned char *answer, int answer_lentgh, unsigned char* res){
    int debut_a_lire, octet, j, is_increment, index, offset, lentgh_res;
    unsigned char pointeur_a_decode[50];
    int longueur_pointeur;
    
    debut_a_lire = HEADER + len_question + QCLASS + QCLASS; /*après l'entete et la section question*/
    is_increment = 0; 
    lentgh_res = 0;
    j = 0;
    
    for(octet = debut_a_lire; octet < answer_lentgh; octet++){
        if (answer[octet] == 0xc0){ 
            if(is_increment > 0) j++;
            offset = answer[++octet] & 0x3ff;
            longueur_pointeur = recursive_searching(answer, pointeur_a_decode, offset, 0);
            index = 0;
            while(index < longueur_pointeur){
                res[j] = pointeur_a_decode[index];
                j++;    
                index++;  
            }
            res[j] = 0x00;
            is_increment++;
            lentgh_res += longueur_pointeur+1;
        }
        else{
            j++;
            res[j] = answer[octet]; 
            lentgh_res++;
        }   
    }
    return lentgh_res;
}

void format_url(unsigned char* url, char *url_formatted){
    int i, j, index, is_increment, len_label;
    i = 0; j = 0, is_increment = 0; 

    while (url[i] != 0x00){
        len_label = url[i];      
        if (len_label < 33){    // si c'est une longeur
            if (is_increment > 0) j++; 
            index = 0;
            while (index < len_label){
                url_formatted[j] = url[++i]; 
                j++;
                index++;
            }      
            url_formatted[j] = 46; // Ascii: 46 = '.'
            is_increment++;
        }
        i++;
    }
    url_formatted[j] = 0x00;
}

int display_one_answer(unsigned char* developed_answer, int lentgh_dev_answer, int offset){
    int i, j, ipv4_lentgh;
    
    unsigned char url [100];
    char url_formatted[100];
    
    unsigned char cname [100];
    char cname_formatted[100];
    
    char *Qtype;

    ipv4_lentgh = 4; /* 4 octet sont reservés pour l'adresse ipv4 */
    
    for (i = 0; developed_answer[offset] != 0x00; i++){
        url[i] = developed_answer[offset];
        offset++;
    }    
    url[i] = 0x00; 
    offset++;
    
    format_url(url, url_formatted); 
    Qtype = decode_Qtype(developed_answer, offset);
    
    printf("%s\t%s\t%s\t", url_formatted, decode_Qclass(), decode_Qtype(developed_answer, offset));
    offset += 2 + 2 + 4 + 2; /* on passe les octets(Qclass(2) + Qtype(2) + TTL(4) + Data_lentgh(2)) après leurs affichage */
    
    /* adress ipv4 */
    if(strcmp(Qtype, "A") == 0){
        for(j = 0; j < ipv4_lentgh; j++){
            if (j< ipv4_lentgh-1)
                printf("%d.", developed_answer[offset]);
            else
                printf("%d", developed_answer[offset]);    
            offset++;
        }
    } 
    /* Canonical name */
    else if (strcmp(Qtype, "CNAME") == 0){       
        for (i = 0; developed_answer[offset] != 0x00; i++){
            cname[i] = developed_answer[offset];
            offset++;
        }        
        cname[i] = 0x00;
        offset++;

        format_url(cname, cname_formatted);
        printf("%s", cname_formatted);
       
    }
    printf("\n");
    return offset;  
}

void display_answer_response(unsigned char *answer, int answer_lentgh){
    unsigned char developed_answer[MAX_TAILLE];
    int i, offset, off, nb_answer, len_developed_answer;

    offset = 0; 
    nb_answer = answer[6] + answer[7];

    // on décode tous les pointeurs contenu dans le tableau answer et on stocke le résultat dans le tableau developed_answer
    len_developed_answer = develop_answer(answer, answer_lentgh, developed_answer);
    
    printf("\n;; ANSWER SECTION:\n");

    if (nb_answer != 0){
        for(i = 0; i < nb_answer; i++){
            off = display_one_answer(developed_answer, len_developed_answer, offset);
            offset = off;
        }
    }
    else
        printf(";; No Answer for the request\n");
    
    printf("\n");
    printf(";; The Decoded Datagram Received from DNS Server:\n\n");
    
    // affichage de l'entete et la section question
    for (int i = 0; i < HEADER+len_question+QCLASS+QTYPE; i++) {

      fprintf(stdout," %.2X", answer[i] & 0xff);

      if (((i+1)%16 == 0) || (i+1 == HEADER+len_question+QCLASS+QTYPE)) {

        /* ceci pour afficher les caracteres ascii aprec l'hexa */
        /* >>> */
        for (int j = i+1 ; j < ((i+16) & ~15); j++) {
          fprintf(stdout,"   ");
        }
        fprintf(stdout,"\t");
        for (int j = i & ~15; j <= i; j++)
          fprintf(stdout,"%c", answer[j] > 31 ? (char)answer[j] : '.');
        /* <<< */
        fprintf(stdout,"\n");
      }
    }

    // affichage de la section reponse
    for (int i = 0; i < len_developed_answer; i++) {

      fprintf(stdout," %.2X", developed_answer[i] & 0xff);

      if (((i+1)%16 == 0) || (i+1 == len_developed_answer)) {

        /* ceci pour afficher les caracteres ascii aprec l'hexa */
        /* >>> */
        for (int j = i+1 ; j < ((i+16) & ~15); j++) {
          fprintf(stdout,"   ");
        }
        fprintf(stdout,"\t");
        for (int j = i & ~15; j <= i; j++)
          fprintf(stdout,"%c",developed_answer[j] > 31 ? (char)developed_answer[j] : '.');
        /* <<< */
        fprintf(stdout,"\n");
      }
    }

}
