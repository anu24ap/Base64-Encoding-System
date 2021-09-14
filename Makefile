all: client server

client: client.c message.h base64.h
	gcc client.c -o client
 
server: server.c message.h
	gcc server.c -o server
