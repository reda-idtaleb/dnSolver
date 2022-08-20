#include "request_builder.h"

void define_header_question_DNSRequest(unsigned char *query){
    int i;
    for(i = 0; i < HEADER; i++){
        if(i == 0)      query[i] = 0x08;
        else if(i == 1) query[i] = 0xbb;
        else if(i == 2) query[i] = 0x01; // parametre 0x01 0x00 (requete recursive)
        else if(i == 5) query[i] = 0x01; // QDcount   0x00 0x01 (1 question)
        else            query[i] = 0x00;
    }   
     
}

void convert_question_DNSFormat(const char *question, unsigned char *query) {
    int noctet = 0;
    int shift = 0;
    size_t pos_noctet = HEADER;
    size_t i;
    for (i = HEADER; i < (HEADER + len_question -1); ++i) { 
        if (question[shift] == '.' || question[shift] == 0) {
            query[pos_noctet] = noctet;
            noctet = 0;
            pos_noctet = i + 1;        
        } 
        else {
            query[i+1] = question[shift];
            noctet++;     
        }
        shift++;        
    }
    query[(HEADER + len_question) - 1] = question[len_question - 2];
}

void define_qtype_question_DNSRequest(unsigned char* query){
    query[HEADER + len_question] = 0x00;     
    query[HEADER + len_question + 1] = 0x01; // adresse IPV4
}

void define_qclass_question_DNSRequest(unsigned char* query){
    query[HEADER + len_question + QCLASS] = 0x00;
    query[HEADER + len_question + QCLASS + 1] = 0x01; // adresse IN
}

void create_DNS_request(const char *question, unsigned char *query, size_t len_request){
    define_header_question_DNSRequest(query);
    convert_question_DNSFormat(question, query);
    define_qtype_question_DNSRequest(query);
    define_qclass_question_DNSRequest(query);
}