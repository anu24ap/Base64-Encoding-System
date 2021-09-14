#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//netwoking related header files
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "message.h"
#include "base64.h"
#include "utils.h"
#define MAXLINE 1000

// stores connected clients ip in this after fork.
char clientname[MAXLINE];

// returns true if M starts with "close"
bool isClose(char *M){
	if(M[0] == 'U'&&M[1] == 'V'&&M[2] == 'V'&&M[3] == 'J'&&M[4] == 'V'&&M[5] == 'A'&&M[6] == '='&&M[7] == '=')
		return 1;
	else
	return 0;		
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     if (argc < 2) {
         fprintf(stderr,"ERROR: no port provided\n");
         exit(1);
     }
     
     portno = atoi(argv[1]);
     sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket
     if (sockfd < 0) //check if socket created
        error("Socket not created!\n");
        
     memset((void*)&serv_addr, 0, sizeof(serv_addr)); //clear serv_addr
   
     serv_addr.sin_family = AF_INET; //set family
     
     //memcpy((void*)&serv_addr.sin_addr.s_addr,(void*)INADDR_ANY);
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     
     serv_addr.sin_port = htons(portno); //handle network byte order(BIG ENDIAN)
     
     //binding the socket to the address and port number specified
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR in binding");
     listen(sockfd,5);
     
     
    //extracts the first connection request on the queue of pending connections for the listening socket, 
    //sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket
    
    int pid;          
    char buffer[MESSAGE_LEN*2];

    
    while(1)
    {
    	struct Message msg; 
    	clilen = sizeof(cli_addr);
    	newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
        if (newsockfd < 0) 
        error("ERROR on accept");
        char client[100];
	inet_ntop(AF_INET, &cli_addr.sin_addr, client, 100);
	printf("\nclient %s connected\n", client);
	
	strcpy(clientname, client);
        //child process is created for serving each new clients
        pid=fork();
        if(pid==0)//child process rec and send
        {
        //receive from client
        	while(1)
        	{
         		n = readMsgFromSocket(newsockfd, &msg);
         		if(n==0)
         		{
         			close(newsockfd);
         			break;
         		}
         		if (n < 0) error("ERROR in reading from socket");
         		printf("\nMessage received from client %s:\n", clientname);
         		printf("%-5s\t%-10s\t%-10s\n", "Type", "Encoded", "Decoded");
         		//decode
			int len=0;
			decode(msg.message,&len, buffer);
			buffer[len] = '\0';
			
			printf("%-5d\t%-10s\t%-10s\n", msg.type, msg.message, buffer);
			bool is_close=0;
			is_close=isClose(msg.message);
			if(is_close){
				printf("%s client sent close.\n", clientname);
				//printf("closing socket....\n");
				close(sockfd);
				exit(1);                                                                       
				// check close server. 
			}
			if(msg.type == TYPE3)
			    break;
			
			msg.type = TYPE2;
			strcpy(msg.message, "Message Received");
			n = writeMsgToSocket(newsockfd, &msg);
			
			//handling the error in writing to socket
			if (n < 0) error("ERROR in writing to socket");
			
        	}
        	exit(0);
        }
        else
        {
        close(newsockfd);//socket is closed by parent
        }
    }//close exterior
   return 0;
}
