***Client-Server Socket Communication with Base64 encoding***

This uses tthe Linux socket API to implement a client-server communication system, using *base64* encoding. 
All the code is written in C, using the socket API.

********USAGE*********

A Makefile is provided.Complie the client and server C files via make.

> make

Open a server on a particular port using the following command. 

> ./server <port-number>

A client can access a particular server with the following commad.

> ./client <server-address> <port-number>


*****ARCHITECTURE*********

Messages are sent/received in fixed size packets. The Header is a 4 byte integer representing the type of packet, followed by the actual messge. The types are as follows:

* TYPE 1 : Packet from client to server
* TYPE 2 : Packet from server to client
* TYPE 3 : Close connection packet from client to server

In this implmentation, size of each packet is 260 Bytes, including the integer. This size is changeable in the *message.h* file. The message is encoded with Base64 encoding by the sender and decoded by the receiver. 

Concurrency, i.e multiple clients being connected to a server is adapted using the fork system call, to spawn a seperate process for each incoming request.

