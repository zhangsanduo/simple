#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "aes.h"

int main(int argc, char const *argv[])
{
    char input[512],output[512],input2[512],output2[512];
    int len;
    if(!strcmp(argv[1], "--en")|| !strcmp(argv[1], "-e"))
    {
        int plen=strlen(argv[2]);
        strcpy(input,argv[2]);
        len=strlen(input);
        fuchextods(input,&output,len);

        int plen2=strlen(argv[3]);
        strcpy(input2,argv[3]);
        len=strlen(input2);
        fuchextods(input2,&output2,len);
        if (plen/2 == 16 && plen2/2 ==16) {
            myAes(output,16,output2);
        } else {
            printf("明文密钥字符长度必须为16！\n");
            exit(0);
        }
    }
    if(!strcmp(argv[1], "--de")|| !strcmp(argv[1], "-d"))
    {
        int clen=strlen(argv[2]);
        strcpy(input,argv[2]);
        len=strlen(input);
        fuchextods(input,&output,len);

        int clen2=strlen(argv[3]);
        strcpy(input2,argv[3]);
        len=strlen(input2);
        fuchextods(input2,&output2,len);

        if (clen/2 == 16 && clen2/2 ==16) {
            myDeAes(output,16,output2);
        } else {
            printf("密文密钥字符长度必须为16！\n");
            exit(0);
        }
    }
    return 0;
}