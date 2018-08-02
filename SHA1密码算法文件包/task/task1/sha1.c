/*
SHA1 tests by Philip Woolford <woolford.philip@gmail.com>
100% Public Domain
 */

#include "sha1.h"
#include "stdio.h"
#include "string.h"
#include "debug.h"

#define SUCCESS 0

/* The suite initialization function.
 * Returns zero on success, non-zero otherwise.
 
 */
int init_suite(
    void
)
{
  return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite(
    void
)
{
  return 0;
}




int HextoDs(char s[])//16进制转10进制
{
    int i,m,temp=0,n;
    m=strlen(s);//十六进制是按字符串传进来的，所以要获得他的长度
    for(i=0;i<m;i++)
    {
        if(s[i]>='A'&&s[i]<='F')//十六进制还要判断他是不是在A-F或者a-f之间a=10。
         n=s[i]-'A'+10;
        else if(s[i]>='a'&&s[i]<='f')
         n=s[i]-'a'+10;
         else n=s[i]-'0';
        temp=temp*16+n;
    }
    return temp;
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
    output[len]=0;
}



int main(int argc,char *argv[])
{
  char result[21];
  char hexresult[41];
  int re=1;
  size_t offset;
  char *string;
  if (argc < 3) {
		printf("You must provide at least 2 parameter, where you specify the action.");
		return 1;}
  char output;
  int len;



  if(!strcmp(argv[1], "--en")|| !strcmp(argv[1], "-e"))
  {
      
	  string = argv[2];
      
  }
  if(!strcmp(argv[1], "--de")|| !strcmp(argv[1], "-d"))
  {
      char input[512];
	 
      strcpy(input,argv[2]);
      len=strlen(input);
      fuchextods(input,&output,len);
	  string = &output;
	  }

   
  dsd_set_log_level(DSD_DEBUG|DSD_INFO);
  



  /* calculate hash */
  SHA1( result, string, strlen(string) );

  /* format the hash for comparison */
  /*字符串16进制转换*/
  for( offset = 0; offset < 20; offset++) {
    sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
  }
  
  printf("%s\n",hexresult);
  



  return re;
}




