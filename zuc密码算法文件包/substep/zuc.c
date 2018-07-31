#include "zuc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int chartoint(char *s)
{
    int n=0;
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
void fuchextods(char intput[],uint8_t output[],int len)
{
    int i,res;
    char a[3];
     for(i=0;i<len;i+=2)//2位为单位处理转换成ASCII后再输出为字符保存在output
    {
        a[0]=intput[i];
        a[1]=intput[i+1];
        a[2]=0;
        res=HextoDs(a);
        output[i/2]=res;
    }
}

int main(int argc, char **argv) {
    int i,j=0;
    char in[200];//16进制输入
    uint8_t out[100];//转换后10进制输出
    uint8_t text[4];//32位转8位密钥
    uint8_t key[16], iv[16],kvin[32];
    if(strlen(argv[1])!=32||strlen(argv[1])!=32)
    {
      printf("输入密钥或初始量必须为32位！！！\n");
      return 0;
    }
    strcpy(kvin,argv[1]);//密钥
    fuchextods(kvin,key,32);
    strcpy(kvin,argv[2]);//初始量
    fuchextods(kvin,iv,32);
    int len=chartoint(argv[3]);//节拍数
    const uint64_t BUFFER_SIZE = 4096 / sizeof(uint32_t);
    uint32_t * buffer = calloc(BUFFER_SIZE, sizeof(uint32_t));
    zuc_context context;
    //memset(key, 0x00, 16);//密钥
    //memset(iv, 0x00, 16);//初始量
    zuc_init(&context, key, iv);//初始化阶段
    zuc_generate_keystream(&context, buffer,len);//工作阶段
   /* for(i=0;i<len;i++)
    {
       printf("密钥%d：%x\n",i+1,buffer[i]); // k iv 全0时为27bede74, 18082da 出流密钥 h
    }*/
    free(buffer);
}
