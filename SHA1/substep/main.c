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




int HextoDs(char s[])//16����ת10����
{
    int i,m,temp=0,n;
    m=strlen(s);//ʮ�������ǰ��ַ����������ģ�����Ҫ������ĳ���
    for(i=0;i<m;i++)
    {
        if(s[i]>='A'&&s[i]<='F')//ʮ�����ƻ�Ҫ�ж����ǲ�����A-F����a-f֮��a=10��
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
     for(i=0;i<len;i+=2)//2λΪ��λ����ת����ASCII�������Ϊ�ַ�������output
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

  printf("initstr=%s\n",argv[2]);

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
  /*�ַ���16����ת��*/
  for( offset = 0; offset < 20; offset++) {
    sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
  }

  printf("sha1_2_1_1_1:%s\n",hexresult);


  return re;
}

