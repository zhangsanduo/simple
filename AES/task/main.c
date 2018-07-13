#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "aes.h"

int main(int argc, char const *argv[])
{
    char input[512],output[512];
    int len;
    if(!strcmp(argv[1], "--en")|| !strcmp(argv[1], "-e"))
    {
        int plen=strlen(argv[2]);
        myAes(argv[2],plen,argv[3]);
    }
    if(!strcmp(argv[1], "--de")|| !strcmp(argv[1], "-d"))
    {
        int clen=strlen(argv[2]);
        strcpy(input,argv[2]);
        len=strlen(input);
        fuchextods(input,&output,len);
        myDeAes(output,clen/2,argv[3]);
    }
    return 0;
}