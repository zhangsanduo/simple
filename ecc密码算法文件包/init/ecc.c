
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int Sign();
extern int Verify();
extern int SM2_RandKey();
extern int mod_n();
extern int set_pra();
extern int show_info();
int main(int argc,char **argv)
{
    if(argc==1)
    {
        printf("please enter ./sm2 -h(--help) for help.\n");
        return 0;
    }

    if(!strcmp(argv[1], "--sign")|| !strcmp(argv[1], "-s"))
    {
        //step.1 check the number of input.
        if(argc<4)
        {
            printf("input error!\n");
            return -2;
        }
        int len2 = strlen(argv[2]);
        int len3 = strlen(argv[3]);

        //step.2 check the length of input.
        if(len2>2048){
            printf("Input characters are too long.\n");
            return -1;
        }
        if(len3!=64)
        {
            printf("Public key must be 64 in length!\n");
            return -1;
        }
        //step.3 check detect input data.
        if((len2%2!=0)||(len3%2!=0))
        {
            printf("Hexadecimal length should be even.\n");
            return -1;
        }
        for(int i=0;i<len2;i++)
        {
            if(!isxdigit(argv[2][i]))
            {
                printf("Data format error.\n");
                return -3;
            }
        }
        for(int i=0;i<len3;i++)
        {
            if(!isxdigit(argv[3][i]))
            {
                printf("Data format error.\n");
                return -3;
            }
        }
        //step.4 Sign Message.
        Sign(argv[2],argv[3]);
    }
    else if(!strcmp(argv[1], "--verify")|| !strcmp(argv[1], "-v"))
    {
        //step.1 check the number of input.
        if(argc<5)
        {
            printf("input error!\n");
            return -2;
        }
        int len2 = strlen(argv[2]);
        int len3 = strlen(argv[3]);
        int len4 = strlen(argv[4]);
        int len5 = strlen(argv[5]);

        //step.2 check the length of input.
        if(len2>2048){
            printf("Input characters are too long.\n");
            return -1;
        }
        if(len3!=64)
        {
            printf("Public key must be 64 in length!\n");
            return -1;
        }
        if(len4!=64)
        {
            printf("Public key must be 64 in length!\n");
            return -1;
        }
        if(len5!=128)
        {
            printf("Public key must be 128 in length!\n");
            return -1;
        }
       //step.3 check detect input data.
        if((len2%2!=0)||(len3%2!=0)||(len4%2!=0)||(len5%2!=0))
        {
            printf("Hexadecimal length should be even.\n");
            return -1;
        }
        for(int i=0;i<len2;i++)
        {
            if(!isxdigit(argv[2][i]))
            {
                printf("Data format error 2.\n");
                return -3;
            }
        }
        for(int i=0;i<len3;i++)
        {
            if(!isxdigit(argv[3][i]))
            {
                printf("Data format error 3.\n");
                return -3;
            }
        }
        for(int i=0;i<len4;i++)
        {
            if(!isxdigit(argv[4][i]))
            {
                printf("Data format error 4.\n");
                return -3;
            }
        }
        for(int i=0;i<len5;i++)
        {
            if(!isxdigit(argv[5][i]))
            {
                printf("Data format error 5.\n");
                return -3;
            }
        }
        //step.4 verify message.                                
        Verify(argv[2],argv[3],argv[4],argv[5]);
    }
    else if(!strcmp(argv[1], "--key")|| !strcmp(argv[1], "-k"))
    {
//        mod_n();
    	SM2_RandKey();
    }
    else if(!strcmp(argv[1], "--help")|| !strcmp(argv[1], "-h"))
    {
        printf("#######   Instructions for use   #######\n");
        printf("-k  (Generate a key pair)\n\n");
        printf("-s  parameter_1:Message\n");
        printf("    parameter_2:User_A private key\n\n");
        printf("-v  parameter_1:Message\n");
        printf("    parameter_2:r\n");
        printf("    parameter_3:s\n");
        printf("    parameter_4:User_A public key\n");
    }
    else if(!strcmp(argv[1], "--var")||!strcmp(argv[1], "-i"))
    {
        if(argc<7)
        {
            return -1;
        }
        int len2 = strlen(argv[2]);
        int len3 = strlen(argv[3]);
        int len4 = strlen(argv[4]);
        int len5 = strlen(argv[5]);
        int len6 = strlen(argv[6]);
        int len7 = strlen(argv[7]);
        if((len2!=64)||(len3!=64)||(len4!=64)||(len5!=64)||(len6!=64)||(len7!=64))
        {
            return -1;
        }
        for(int i=0;i<len5;i++)
        {
            if((!isxdigit(argv[2][i]))||(!isxdigit(argv[3][i]))||(!isxdigit(argv[4][i]))||
                (!isxdigit(argv[5][i]))||(!isxdigit(argv[6][i]))||(!isxdigit(argv[7][i])))
            {
                printf("Data format error 5.\n");
                return -3;
            }
        }
        set_pra(argv[2],argv[3],argv[4],argv[5],argv[6],argv[7]);
    }
    else if(!strcmp(argv[1], "--show")||!strcmp(argv[1], "-ls"))
    {
        show_info();
    }
    else
    {
        printf("please enter ./sm2 -h(--help) for help.\n");
    }
	return 0;
}
