CC      = gcc

all: clean mainClient mainServer

mainClient: client.o mainClient.o
	$(CC) -o mainClient client.o mainClient.o

mainClient.o: mainClient.c client.h
	$(CC) -c  mainClient.c

client.o: client.c client.h
	$(CC) -c  client.c
	
mainServer: server.o mainServer.o
	$(CC) -o mainServer server.o mainServer.o

mainServer.o: mainServer.c server.h
	$(CC) -c  mainServer.c

server.o: server.c server.h
	$(CC) -c  server.c
		
clean:
		rm -f *.o 
		rm -f mainServer
		rm -f mainClient
	
	


