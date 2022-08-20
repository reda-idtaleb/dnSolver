CC      = gcc
CFLAGS  = -Wall -Wextra
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

all: clean dns_main request_builder.o response_analyser.o client.o


request_builder.o: dns/request_builder.c dns/request_builder.h 
	$(CC) -c  dns/request_builder.c

response_analyser.o: dns/response_analyser.c dns/response_analyser.h dns/request_builder.h 
	$(CC) -c  dns/response_analyser.c

dns_main: dns_main.o request_builder.o response_analyser.o client.o
	$(CC) -o dnsolver client.o response_analyser.o request_builder.o dns_main.o

dns_main.o: dns/dns_main.c dns/request_builder.h dns/response_analyser.h udp/client.h
	$(CC) -c  dns/dns_main.c

client.o: udp/client.c udp/client.h
	$(CC) -c  udp/client.c


clean: 
	rm -f *.o 
	rm -f request_builder	
	rm -f dnsolver
	rm -f server
	rm -f client
	

	
