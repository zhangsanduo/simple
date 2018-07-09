#include <stdio.h>
#include "rsa.h"
#include <string.h>
#include <stdlib.h>
long long chartoint(char *s)
{
    long long n=0;
    char *p=s;
    while(*p)
    {
        n=n*10+(*p-'0'); //把字符转换成数值
        p++;
 	}
 	return n;
}
int HextoDs(char s[])//16进制转10进制
{
    int i,m,temp=0,n;
    m=strlen(s);//十六进制是按字符串传进来的，所以要获得他的长度
    for(i=0;i<m;i++)
    {
        if(s[i]>='A'&&s[i]<='F')//十六进制还要判断他是不是在A-F或者a-f之间a=10。。
         n=s[i]-'A'+10;
        else if(s[i]>='a'&&s[i]<='f')
         n=s[i]-'a'+10;
         else n=s[i]-'0';
        temp=temp*16+n;
    }
    return temp;
}
int hextest(char intput[],int len)//测试HEX格式
{
    int i,test=1;
    for(i=0;i<len;i+=2)
    {
        if(isxdigit(intput[i])==0)
        {
            test=0;
            break;
        }
    }
    return test;
}
void fuchextods(char intput[],char output[],int len)
{
    int i;
    char a[3],res;
     for(i=0;i<len;i+=2)//2位为单位处理转换成ASCII后再输出为字符保存在output
    {
        a[0]=intput[i];
        a[1]=intput[i+1];
        a[2]=0;
        res=HextoDs(a);
        output[i/2]=res;
    }
    output[len/2]=0;//字符串终止
}
int main(int argc, char **argv)
{
  struct public_key_class pub[1];
  struct private_key_class priv[1];
  char message[512],intput[512];
  char number[20];
  int i,len;
  if(!strcmp(argv[1], "--gen")|| !strcmp(argv[1], "-g"))
{
    rsa_gen_keys(pub, priv, PRIME_SOURCE_FILE);

  printf("私钥:\n n: %lld\n d: %lld\n", (long long)priv->modulus, (long long) priv->exponent);
  printf("公钥:\n n: %lld\n e: %lld\n", (long long)pub->modulus, (long long) pub->exponent);
}
  if(!strcmp(argv[1], "--encrypt")|| !strcmp(argv[1], "-e"))
{
  strcpy(number,argv[2]);
  pub->exponent=chartoint(number);
  strcpy(number,argv[3]);
  pub->modulus=chartoint(number);
  strcpy(intput, argv[4]);
  len=strlen(intput);
        if(len%2!=0)//判断是否为双数位
        {
            printf("16进制不符合要求");
            return 0;
        }
         if(hextest(intput,len)==1)
        {
            fuchextods(intput,&message,len);
        }
        else
        {
            printf("16进制不符合要求");
            return 0;
        }
  //加密
  long long *encrypted = rsa_encrypt(message, strlen(message), pub);
  if (!encrypted){
    fprintf(stderr, "Error in encryption!\n");
    return 1;
  }
  printf("加密:\n");
  for(i=0; i < strlen(message); i++){
    printf("%x ", (long long)encrypted[i]);
  } 
 printf("\n"); 
  free(encrypted);
}
  if(!strcmp(argv[1], "--decrypt")|| !strcmp(argv[1], "-d"))
{
  //解密
  strcpy(number,argv[2]);
  priv->exponent=chartoint(number);
  strcpy(number,argv[3]);
  priv->modulus=chartoint(number);
  long long *ciphertext=malloc(argc-3);
  int j=0;
  len=0;
  for(i=4;i<argc;i++)
{
  ciphertext[j++]=HextoDs(argv[i]);
   len=j;
}
  long long *decrypted =rsa_decrypt(ciphertext,len,priv);
  if (!decrypted){
    fprintf(stderr, "Error in decryption!\n");
    return 1;
  }
  printf("解密:\n");
  for(i=0; i < len; i++){
    printf("%x ", (long long)decrypted[i]);
  }  
  printf("\n");
 free(decrypted);
}
  return 0;
}
