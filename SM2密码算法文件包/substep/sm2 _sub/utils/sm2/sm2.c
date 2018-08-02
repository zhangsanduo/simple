#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
extern int SM2_RandKey();
extern int ENCRYPT();
extern int DECRYPT();
int main(int argc,char **argv)
{
    if(argc==1)
    {
        printf("please enter ./sm2 -h(--help) for help.\n");
        return 0;
    }

    if(!strcmp(argv[1], "--en")|| !strcmp(argv[1], "-e"))
    {
        //step.1 check the number of input.
        if(argc!=4)
        {
            printf("input error!\n");
            return -2;
        }
        //step.2 check the length of input.
        int m_len = strlen(argv[2]);
        if(m_len>2048){
            printf("Input characters are too long.\n");
            return -1;
        }
        int pb_len = strlen(argv[3]);
        if(pb_len!=128)
        {
            printf("Public key must be 128 in length!\n");
            return -1;
        }
        //step.3 check detect input data.
        if((m_len%2!=0)||(pb_len%2!=0))
        {
            printf("Hexadecimal length should be even.\n");
            return -1;
        }
        for(int i=0;i<m_len;i++)
        {
            if(!isxdigit(argv[2][i]))
            {
                printf("Data format error.\n");
                return -3;
            }
        }
        for(int i=0;i<pb_len;i++)
        {
            if(!isxdigit(argv[3][i]))
            {
                printf("Data format error.\n");
                return -3;
            }
        }
        //step.4 Encrypted data.
        ENCRYPT(argv[2],argv[3]);
    }
    else if(!strcmp(argv[1], "--de")|| !strcmp(argv[1], "-d"))
    {
        //step.1 check the number of input.
        if(argc<4)
        {
            printf("input error!\n");
            return -2;
        }
        int c_len = strlen(argv[2]);
        //step.2 check the length of input.
        if(c_len<192)
        {
            printf("The ciphertext length must be greater than 192!\n");
            return -1;
        }
        int b_len = strlen(argv[3]);
        if(b_len!=64)
        {
            printf("B prikey lenght must be 64!\n");
            return -1;            
        }
        //step.3 check detect input data.
        if((c_len%2!=0)||(b_len%2!=0))
        {
             printf("Hexadecimal length should be even.\n");
            return -1;           
        }
        for(int i=0;i<c_len;i++)
        {
            if(!isxdigit(argv[2][i]))
            {
                printf("Data format error.\n");
                return -3;
            }            
        }
        for(int i=0;i<b_len;i++)
        {
            if(!isxdigit(argv[3][i]))
            {
                printf("Data format error.\n");
                return -3;
            }            
        }
        //step.4 Decrypted data.
        DECRYPT(argv[2],argv[3]);
    }   
    else if(!strcmp(argv[1], "--key")|| !strcmp(argv[1], "-k"))
    {
        //Generate a random key
        SM2_RandKey(1);
    }
    else if(!strcmp(argv[1], "--help")|| !strcmp(argv[1], "-h"))
    {
        printf("#######   Instructions for use   #######\n");
        printf("-k  (Generate a key pair)\n\n");
        printf("-e  parameter_1:Message\n");
        printf("    parameter_2:public key\n\n");
        printf("-d  parameter_1:Cipher text\n");
        printf("    parameter_2:User private key\n");
    }
    else
    {
        printf("please enter ./sm2 -h(--help) for help.\n");
    }
	return 0;
}
