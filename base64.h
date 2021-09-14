#ifndef BASE64_H
#define BASE64_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Helper function
//prints binary rep. of an integer
void printBin(int val){
    for(int i=31; i>=0; i--){
        printf("%d", (val>>i)&1);
        if(i%8 == 0) printf(" ");
    }
    printf("\n");
}

//encodes a 6bit group into base64 character
char encode64_grp(int val){
    char target;
    if(val < 26)
        target = 'A'+ val;
    else if(val < 52)
        target = 'a' +(val-26);
    else if(val < 62)
        target = '0' + (val-52);
    else if(val ==  62)
        target = '+';
    else target = '/';

    return target;
}

//decodes a base64 char into int value
int decode64(char c){
    if(c>='A' && c<='Z') return c-'A';
    else if(c>='a' && c<='z') return c-'a'+26;
    else if(c>='0' && c<='9') return c-'0'+52;
    else if(c == '+') return 62;
    else if(c == '/') return 63;
    else if(c=='=') return 0;
    else return -1;
}

//encodes binary data rep. by data pointer with len_data length
//heap allocates the buffer and returns null terminated string of encoded data
void encode(char* data, int len_data, char* buffer){
    int padding = len_data%3;
    if(padding != 0) padding = 3-padding;
   
    int blocks = (len_data+padding)/3;

    char* tmp = data;
    int index=0;
    while(tmp-data + 3 <= len_data){
        int val = (tmp[0]<<16) | (tmp[1]<<8) | tmp[2];
        //encode the 4 groups of 6 bits
        for(int i=3; i>=0; i--){
            int grp = ((val>>(i*6))&0x3F);
            buffer[index++] = encode64_grp(grp);
        }
        tmp += 3;
    }
    
    if(padding > 0){
        char tmp_blck[3] = {'\0', '\0', '\0'};
        tmp_blck[0] = *tmp;
        if(padding == 1) tmp_blck[1] = *(tmp+1);

        int val = (tmp_blck[0]<<16) | (tmp_blck[1]<<8) | tmp_blck[2];
        for(int i=3; i>=0; i--){
            int grp = ((val>>(i*6))&0x3F);
            buffer[index++] = encode64_grp(grp);
        }
        buffer[index-1] = '=';
        if(padding==2) buffer[index-2] = '=';
    }

    buffer[index] = '\0';
}


//decodes null terminated string of base64 string and returns the
//heap allocated buffer, stores length of buffer in decode_len 
void decode(char* data, int* decode_len, char* buffer){
    int len_data = strlen(data);
    int grps = len_data/4;
    *decode_len = grps*3;

    char* pd = data,*pb = buffer;
    while(pd-data < len_data){
        int val = (decode64(pd[0])&0x3F) << 18 |
                (decode64(pd[1])&0x3F) << 12 |
                (decode64(pd[2])&0x3F) << 6 |
                (decode64(pd[3])&0x3F);
        pb[0] = (val>>16)&0xFF;
        pb[1] = (val>>8)&0xFF;
        pb[2] =  val&0xFF;

        pb += 3;
        pd += 4;
        
    }

}

#endif