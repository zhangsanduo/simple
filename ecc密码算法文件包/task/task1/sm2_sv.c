
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kdf.h"

unsigned char SM2_p[32] =
{0x85,0x42,0xd6,0x9e,0x4c,0x04,0x4f,0x18,0xe8,0xb9,0x24,0x35,0xbf,0x6f,0xf7,0xde,
0x45,0x72,0x83,0x91,0x5c,0x45,0x51,0x7d,0x72,0x2e,0xdb,0x8b,0x08,0xf1,0xdf,0xc3};

unsigned char SM2_a[32] =
{0x78,0x79,0x68,0xb4,0xfa,0x32,0xc3,0xfd,0x24,0x17,0x84,0x2e,0x73,0xbb,0xfe,0xff,
0x2f,0x3c,0x84,0x8b,0x68,0x31,0xd7,0xe0,0xec,0x65,0x22,0x8b,0x39,0x37,0xe4,0x98};

unsigned char SM2_b[32] = 
{0x63,0xe4,0xc6,0xd3,0xb2,0x3b,0x0c,0x84,0x9c,0xf8,0x42,0x41,0x48,0x4b,0xfe,0x48,
0xf6,0x1d,0x59,0xa5,0xb1,0x6b,0xa0,0x6e,0x6e,0x12,0xd1,0xda,0x27,0xc5,0x24,0x9a};

unsigned char SM2_Gx[32]=
{0x42,0x1d,0xeb,0xd6,0x1b,0x62,0xea,0xb6,0x74,0x64,0x34,0xeb,0xc3,0xcc,0x31,0x5e,
0x32,0x22,0x0b,0x3b,0xad,0xd5,0x0b,0xdc,0x4c,0x4e,0x6c,0x14,0x7f,0xed,0xd4,0x3d};

unsigned char SM2_Gy[32]=
{0x06,0x80,0x51,0x2b,0xcb,0xb4,0x2c,0x07,0xd4,0x73,0x49,0xd2,0x15,0x3b,0x70,0xc4,
0xe5,0xd7,0xfd,0xfc,0xbf,0xa3,0x6e,0xa1,0xa8,0x58,0x41,0xb9,0xe4,0x6e,0x09,0xa2};

unsigned char SM2_n[32] =
{0x85,0x42,0xd6,0x9e,0x4c,0x04,0x4f,0x18,0xe8,0xb9,0x24,0x35,0xbf,0x6f,0xf7,0xdd,
0x29,0x77,0x20,0x63,0x04,0x85,0x62,0x8d,0x5a,0xe7,0x4e,0xe7,0xc3,0x2e,0x79,0xb7};

unsigned char SM2_h[32]=
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};


big para_p,para_a,para_b,para_n,para_Gx,para_Gy,para_h;
epoint *G,*nG;
miracl *mip;

void rand32h(char d[64])
{
    int result;
    unsigned char i,n3;
	struct timeval tpstart;
	gettimeofday(&tpstart,NULL);
	srand(tpstart.tv_usec);

    for (n3=i=0;i<64;)
    {
        if (n3==0) {result = rand(); n3=3;}
        d[i++]=result&15;
        result>>=4; --n3;
    }
}

void print_hex(char *d, int n,unsigned char rand_out[])
{
    const char hextable[]="0123456789ABCDEF";
    int i=0;
    while(n--)
    {
        rand_out[i++]=hextable[*d++&15];
//        putchar(hextable[*d++&15]);
    }
    rand_out[i]='\0';
}

int HextoDs(char s[])
{
    int i,m,temp=0,n;
    m=strlen(s);
    for(i=0;i<m;i++)
    {
        if(s[i]>='A'&&s[i]<='F')
         n=s[i]-'A'+10;
        else if(s[i]>='a'&&s[i]<='f')
         n=s[i]-'a'+10;
         else n=s[i]-'0';
        temp=temp*16+n;
    }
    return temp;
}

void fuchextods(char intput[],unsigned char output[],int len)
{
    int i,res;
    char a[3];
     for(i=0;i<len;i+=2)
    {
        a[0]=intput[i];
        a[1]=intput[i+1];
        a[2]=0;
        res=HextoDs(a);
        output[i/2]=res;
    }
}

int SM2_Init(){
	para_Gx=mirvar(0);
	para_Gy=mirvar(0);
	para_p=mirvar(0);
	para_a=mirvar(0);
	para_b=mirvar(0);
	para_n=mirvar(0);
	para_h=mirvar(0);
	bytes_to_big(SM2_NUMWORD,SM2_Gx,para_Gx);
	bytes_to_big(SM2_NUMWORD,SM2_Gy,para_Gy);
	bytes_to_big(SM2_NUMWORD,SM2_p,para_p);
	bytes_to_big(SM2_NUMWORD,SM2_a,para_a);
	bytes_to_big(SM2_NUMWORD,SM2_b,para_b);
	bytes_to_big(SM2_NUMWORD,SM2_n,para_n);
	bytes_to_big(SM2_NUMWORD,SM2_h,para_h);
	ecurve_init(para_a,para_b,para_p,MR_PROJECTIVE);
	G=epoint_init();
	nG=epoint_init();
	if (!epoint_set(para_Gx,para_Gy,0,G))//initialise point G
	{
		return ERR_ECURVE_INIT;
	}
	ecurve_mult(para_n,G,nG);
	if (!point_at_infinity(nG)) //test if the order of the point is n
	{
		return ERR_ORDER;
	}
	return 0;
}

int Test_Point(epoint* point)
{
	big x,y,x_3,tmp;
	x=mirvar(0);
	y=mirvar(0);
	x_3=mirvar(0);
	tmp=mirvar(0);
	//test if y^2=x^3+ax+b
	epoint_get(point,x,y);
	power (x, 3, para_p, x_3); //x_3=x^3 mod p
	multiply (x, para_a,x); //x=a*x
	divide (x, para_p, tmp); //x=a*x mod p , tmp=a*x/p
	add(x_3,x,x); //x=x^3+ax
	add(x,para_b,x); //x=x^3+ax+b
	divide(x,para_p,tmp); //x=x^3+ax+b mod p
	power (y, 2, para_p, y); //y=y^2 mod p
	if(mr_compare(x,y)!=0)
		return ERR_NOT_VALID_POINT;
	else
		return 0;
}

int Test_PubKey(epoint *pubKey)
{
	big x,y,x_3,tmp;epoint *nP;
	x=mirvar(0);
	y=mirvar(0);
	x_3=mirvar(0);
	tmp=mirvar(0);
	nP=epoint_init();
	//test if the pubKey is the point at infinity
	if (point_at_infinity(pubKey))// if pubKey is point at infinity, return error;
	return ERR_INFINITY_POINT;
	//test if x<p and y<p both hold
	epoint_get(pubKey,x,y);
	if((mr_compare(x,para_p)!=-1) || (mr_compare(y,para_p)!=-1))
		return ERR_NOT_VALID_ELEMENT;
	if(Test_Point(pubKey)!=0)
		return ERR_NOT_VALID_POINT;
	//test if the order of pubKey is equal to n
	ecurve_mult(para_n,pubKey,nP); // nP=[n]P
	if (!point_at_infinity(nP)) // if np is point NOT at infinity, return error;
	return ERR_ORDER;
	return 0;
}

int Test_Zero(big x)
{
	big zero;
	zero=mirvar(0);
	if(mr_compare(x,zero)==0)return 1;
	else return 0;
}

int Test_n(big x)
{
	// bytes_to_big(32,SM2_n,n);
	if(mr_compare(x,para_n)==0)
		return 1;
	else return 0;
}

int Test_Range(big x)
{
	big one,decr_n;
	one=mirvar(0);
	decr_n=mirvar(0);
	convert(1,one);
	decr(para_n,1,decr_n);
	if( (mr_compare(x,one) < 0)| (mr_compare(x,decr_n)>0) )
		return 1;
	return 0;
}

int Test_Null(unsigned char array[],int len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		if (array[i]!=0x00)
			return 0;
	}
	return 1;
}

int SM2_KeyGeneration(unsigned char PriKey[],unsigned char Px[],unsigned char Py[])
{
	int i=0;
	big d,PAx,PAy;
	epoint *PA;
	SM2_Init();
	PA=epoint_init();
	d=mirvar(0);
	PAx=mirvar(0);
	PAy=mirvar(0);
	bytes_to_big(SM2_NUMWORD,PriKey,d);
	ecurve_mult(d,G,PA);
	epoint_get(PA,PAx,PAy);
	big_to_bytes(SM2_NUMWORD,PAx,Px,TRUE);
	big_to_bytes(SM2_NUMWORD,PAy,Py,TRUE);
	i=Test_PubKey(PA);
	if(i)
		return i;
	else
		return 0;
}

//sm2 sign message.
int SM2_Sign(unsigned char *message,int len,unsigned char ZA[],unsigned char rands[],unsigned
		char d[],unsigned char R[],unsigned char S[])
{
	unsigned char hash[SM3_len/8];
	int M_len=len+SM3_len/8;
	unsigned char *M=NULL;
	int i;
	unsigned char rand[64],out[64],x1y1[64];
	big dA,r,s,e,k,KGx,KGy;
	big rem,sem,rk,z1,z2,str;
	epoint *KG;
	unsigned char hex_rand[32]={
		0x6c,0xb2,0x8d,0x99,0x38,0x5c,0x17,0x5c,0x94,0xf9,0x4e,0x93,0x48,0x17,0x66,0x3f,
		0xc1,0x76,0xd9,0x25,0xdd,0x72,0xb7,0x27,0x26,0x0d,0xba,0xae,0x1f,0xb2,0xf9,0x6f};
	return 0;
}

int SM2_Verify(unsigned char *message,int len,unsigned char ZA[],unsigned char Px[],unsigned
		char Py[],unsigned char R[],unsigned char S[]){
	unsigned char hash[SM3_len/8];
	int M_len=len+SM3_len/8;
	unsigned char *M=NULL;
	int i;
	big PAx,PAy,r,s,e,t,rem,x1,y1;
	big RR;
	epoint *PA,*sG,*tPA;
	unsigned char R1[32],x1y1[64];
	i=SM2_Init();
	if(i) return i;
	PAx=mirvar(0);
	PAy=mirvar(0);
	r=mirvar(0);
	s=mirvar(0);
	e=mirvar(0);
	t=mirvar(0);
	x1=mirvar(0);
	y1=mirvar(0);
	rem=mirvar(0);
	RR=mirvar(0);
	PA=epoint_init();
	sG=epoint_init();
	tPA=epoint_init();
	bytes_to_big(SM2_NUMWORD,Px,PAx);
	bytes_to_big(SM2_NUMWORD,Py,PAy);
	bytes_to_big(SM2_NUMWORD,R,r);
	bytes_to_big(SM2_NUMWORD,S,s);
	if (!epoint_set(PAx,PAy,0,PA))//initialise public key
	{
		return ERR_PUBKEY_INIT;
	}
	//step1: test if r belong to [1,n-1]
	if (Test_Range(r))
	{	
		return ERR_OUTRANGE_R;
	}
	//step2: test if s belong to [1,n-1]
	if (Test_Range(s))
	{
		return ERR_OUTRANGE_S;
	}
	//step3,generate M
	M=(char *)malloc(sizeof(char)*(M_len+1));
	memcpy(M,ZA,SM3_len/8);
	memcpy(M+SM3_len/8,message,len);
	//step4,generate e=H(M)
	SM3_256(M, M_len, hash);
	bytes_to_big(SM3_len/8,hash,e);
	//step5:generate t
	add(r,s,t);
	power(t,1,para_n,rem);
	if( Test_Zero(rem))
	{
		return ERR_GENERATE_T;
	}
	//step 6: generate(x1,y1)
	ecurve_mult(s,G,sG);
	ecurve_mult(rem,PA,tPA);
	ecurve_add(sG,tPA);
	epoint_get(tPA,x1,y1);
	//step7:generate RR
	add(e,x1,RR);
	power(RR,1,para_n,rem);
	free(M);
	if(mr_compare(rem,r)==0)
	{
		return 0;
	}
	else
	{
		return ERR_DATA_MEMCMP;
	}
}

int SM2_RandKey(int index,unsigned char keys[])
{
	//step.1 init
	unsigned char d[64];   
    unsigned char key[64];
    int long_len = 64;
    unsigned char out[long_len/2];
    big a_ks,a_x,a_y,str;
    epoint *a_kg;
	mip= mirsys(1000, 16);
	mip->IOBASE=16;
	SM2_Init();
    a_ks=mirvar(0);
    a_x=mirvar(0);
    a_y=mirvar(0);
    str=mirvar(0);
    a_kg=epoint_init();

    //step.2 generate random private key
    while(1)
    {
    	//step.2.1  rand32h
    	rand32h(d); //产生随机数
    	//step.2.2 change to hex
    	print_hex(d,64,key);
    	//step.2.3  change style to hex byte
    	fuchextods(key,out,long_len);
    	//step.2.4 byte to big
    	bytes_to_big(32,out,a_ks);
    	//step.2.5 calc kg
//    	power(str,1,para_n,a_ks);
    	int flage = SM2_KeyGeneration_enc(a_ks,a_kg);
    	//step.6 check kg
    	if(flage==0){
    		break;
    	}else{
    		continue;
    	}    		
    }

    if(index==2)
    {
    	memcmp(keys,out,SM2_NUMWORD);
    	return 0;
    }

    //step.3 calc public key
    unsigned char kGxy[SM2_NUMWORD*2]={0};//SM2_NUMWORD 32
	epoint_get(a_kg,a_x,a_y);
//	power(a_x,1,para_n,str);
	big_to_bytes(SM2_NUMWORD,a_x,kGxy,1);
//	power(a_y,1,para_n,str);
	big_to_bytes(SM2_NUMWORD,a_y,kGxy+SM2_NUMWORD,1);

	//step.4 output result
	printf("private key:");
    for(int i=0;i<SM2_NUMWORD;i++)
    {
		printf("%02x", out[i]);
    }
    printf("\n");

	printf("public key:");
	for(int i=0;i<SM2_NUMWORD*2;i++)
	{
		printf("%02x", kGxy[i]);
	}
	printf("\n");
	return 0;
}

int Sign(unsigned char in_message[],unsigned char in_prikey[])
{
    big ks,x,y;
    epoint *kg;
	mip= mirsys(1000, 16);
	mip->IOBASE=16;
	SM2_Init();
    ks=mirvar(0);
    x=mirvar(0);
    y=mirvar(0);
    kg=epoint_init();
    int i;
	//the private key
	unsigned char dA[32];
	int dA_len = strlen(in_prikey);
	fuchextods(in_prikey,dA,dA_len);
	bytes_to_big(32,dA,ks);
	SM2_KeyGeneration_enc(ks,kg);
	epoint_get(kg,x,y);
	unsigned char rand[32];
	//the public key
	unsigned char xA[32],yA[32];
	big_to_bytes(32,x,xA,1);
	big_to_bytes(32,y,yA,1);
	unsigned char r[32],s[32];// Signature
	unsigned char IDA[18]={
		0x41,0x4c,0x49,0x43,0x45,0x31,0x32,0x33,0x40,0x59,0x41,0x48,0x4f,0x4f,0x2e,0x43,
		0x4f,0x4d};
	int IDA_len=18;
	unsigned char ENTLA[2]={0x00,0x90};//the length of userA's identification,presentation in ASCII code
	int len=strlen(in_message);//the length of message
	unsigned char message[len];//the message to be signed
	fuchextods(in_message,message,len);
	unsigned char ZA[SM3_len/8];//ZA=Hash(ENTLA|| IDA|| a|| b|| Gx || Gy || xA|| yA)
	int msg_len = IDA_len + 2 + SM2_NUMWORD*6;
	unsigned char Msg[msg_len]; //212=IDA_len+2+SM2_NUMWORD*6
	int temp;

	temp=SM2_KeyGeneration(dA,xA,yA);
	if(temp)
		return temp;

	// ENTLA|| IDA|| a|| b|| Gx || Gy || xA|| yA
	memcpy(Msg,ENTLA,2);
	memcpy(Msg+2,IDA,IDA_len);
	memcpy(Msg+2+IDA_len,SM2_a,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD,SM2_b,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*2,SM2_Gx,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*3,SM2_Gy,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*4,xA,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*5,yA,SM2_NUMWORD);

	SM3_256(Msg,msg_len,ZA);

	temp=SM2_Sign(message,len/2,ZA,rand,dA,r,s);
	if(temp)
		return temp;
//	printf("r is:");
    for(int i=0;i<SM2_NUMWORD;i++)
    {
        printf("%02x",r[i]);
    }
    printf("\n");
//	printf("s is:");
    for(int i=0;i<SM2_NUMWORD;i++)
    {
        printf("%02x",s[i]);
    }
    printf("\n");
    return 0;
}

int Verify(unsigned char v_message[],unsigned char v_r[],unsigned char v_s[],unsigned char v_pubkey[])
{
	int i;
	int len_key = strlen(v_pubkey);
	unsigned char xAyA[len_key/2];
	fuchextods(v_pubkey,xAyA,len_key);
	//the public key
	unsigned char xA[32],yA[32];
	memcpy(xA,xAyA,SM2_NUMWORD);
	memcpy(yA,xAyA+SM2_NUMWORD,SM2_NUMWORD);
	unsigned char r[32],s[32];// Signature
	unsigned char IDA[18]={
		0x41,0x4c,0x49,0x43,0x45,0x31,0x32,0x33,0x40,0x59,0x41,0x48,0x4f,0x4f,0x2e,0x43,
		0x4f,0x4d};
	int IDA_len=18;
	unsigned char ENTLA[2]={0x00,0x90};//the length of userA's identification,presentation in ASCII code
	int len=strlen(v_message);//the length of message
	unsigned char message[len/2];//the message to be signed
	fuchextods(v_message,message,len);
	
	unsigned char ZA[SM3_len/8];//ZA=Hash(ENTLA|| IDA|| a|| b|| Gx || Gy || xA|| yA)
	int msg_len = IDA_len + 2 + SM2_NUMWORD*6;
	unsigned char Msg[msg_len]; 
	int temp;
	fuchextods(v_r,r,SM2_NUMWORD*2);
	fuchextods(v_s,s,SM2_NUMWORD*2);
	miracl *mip=mirsys(10000,16);
	mip->IOBASE=16;
	// ENTLA|| IDA|| a|| b|| Gx || Gy || xA|| yA
	memcpy(Msg,ENTLA,2);
	memcpy(Msg+2,IDA,IDA_len);
	memcpy(Msg+2+IDA_len,SM2_a,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD,SM2_b,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*2,SM2_Gx,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*3,SM2_Gy,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*4,xA,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*5,yA,SM2_NUMWORD);
	SM3_256(Msg,msg_len,ZA);

	temp=SM2_Verify(message,len/2,ZA,xA,yA,r,s);
	if(temp){
		printf("Verify error!\n");
		return temp;
	}
	printf("Verify sucess!\n");
	return 0;
}

int mod_n()
{
	mip= mirsys(1000, 16);
	mip->IOBASE=16;
	SM2_Init();
	unsigned char results[32];
	power(para_p,1,para_n,para_a);
	big_to_bytes(SM2_NUMWORD,para_a,results,TRUE);
	for(int i=0;i<SM2_NUMWORD;i++)
	{
		printf("%02x", results[i]);
	}
	printf("\n");
}
