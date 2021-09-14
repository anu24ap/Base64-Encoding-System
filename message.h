#ifndef MESSAGE_H
#define MESSAGE_H

#include <string.h>
#include <unistd.h>

#define MESSAGE_LEN 256

enum MESSAGE_TYPE{
    TYPE1=1, TYPE2, TYPE3
};

struct Message{
    enum MESSAGE_TYPE type;
    char message[MESSAGE_LEN];
};

int writeMsgToSocket(int sockfd, struct Message* msg){
    int n=write(sockfd,(char*)msg, sizeof(struct Message));
    return n;

}

int readMsgFromSocket(int sockfd, struct Message* msg){
    int n=read(sockfd,(char*)msg, sizeof(struct Message));;
    return n;
}

#endif