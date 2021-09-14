#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//generic method to output error and exit
void error(char* msg){
    perror(msg);
    exit(1);
}

void readLine(char* buffer, int size){
    fgets(buffer, size, stdin);
    //remove newline character
    char* pos;
    if((pos=strchr(buffer,'\n')) != NULL)
        *pos = '\0';
    
}

#endif