
#include "miracl.h"
#include "mirdef.h"
#include "kdf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

int SM2_KeyGeneration_enc(big priKey,epoint *pubKey)
{
	int i=0;
	big x,y;
	x=mirvar(0);
	y=mirvar(0);
	//calc public key
	ecurve_mult(priKey,G,pubKey);//G is base point
	//Save point coordinates
	epoint_get(pubKey,x,y);
	//Verify that the public key is on the curve.(Declared in sm2_sv.c)
	if(Test_PubKey(pubKey)!=0)
		return 1;
	else
		return 0;
}

int SM2_Encrypt(unsigned char* randK,epoint *pubKey,unsigned char M[],int klen,unsigned char C[])
{
	//step.1 init
	big C1x,C1y,x2,y2,rand;
	epoint *C1,*kP,*S;
	int i=0;
	unsigned char x2y2[SM2_NUMWORD*2]={0};
	SM3_STATE md;
	C1x=mirvar(0);
	C1y=mirvar(0);
	x2=mirvar(0);
	y2=mirvar(0);
	rand=mirvar(0);
	C1=epoint_init();
	kP=epoint_init();
	S=epoint_init();

	//Step.2 calculate C1=[k]G=(rGx,rGy)
	bytes_to_big(SM2_NUMWORD,randK,rand);
	ecurve_mult(rand,G,C1); //C1=[k]G
	epoint_get(C1,C1x,C1y);
	big_to_bytes(SM2_NUMWORD,C1x,C,1);
	big_to_bytes(SM2_NUMWORD,C1y,C+SM2_NUMWORD,1);

	//Step.3 test if S=[h]pubKey if the point at infinity
	ecurve_mult(para_h,pubKey,S);
	if (point_at_infinity(S))// if S is point at infinity, return error;
	{
		printf("[h]P is infinity.\n");
		return ERR_INFINITY_POINT;
	}

	//Step.4 calculate [k]PB=(x2,y2)
	ecurve_mult(rand,pubKey,kP); //kP=[k]P
	epoint_get(kP,x2,y2);

	//Step.5 KDF(x2||y2,klen)
	big_to_bytes(SM2_NUMWORD,x2,x2y2,1);
	big_to_bytes(SM2_NUMWORD,y2,x2y2+SM2_NUMWORD,1);
	SM3_KDF(x2y2 ,SM2_NUMWORD*2, klen*8,C+SM2_NUMWORD*2);
	if(Test_Null(C+SM2_NUMWORD*3,klen)!=0)
	{
		printf("C2 is not on the curve.\n");
		return ERR_ARRAY_NULL;
	}

	//Step.6 C2=M^t
	for(i=0;i<klen;i++)
	{
		C[SM2_NUMWORD*2+i]=M[i]^C[SM2_NUMWORD*2+i];
	}

	//Step.7 C3=hash(x2,M,y2)
	SM3_init(&md);
	SM3_process(&md,x2y2,SM2_NUMWORD);
	SM3_process(&md,M,klen);
	SM3_process(&md,x2y2+SM2_NUMWORD,SM2_NUMWORD);
	SM3_done(&md,C+SM2_NUMWORD*2+klen);
	return 0;
}

int SM2_Decrypt(big dB,unsigned char C[],int Clen,unsigned char M[])
{
	//step.1 init
	SM3_STATE md;
	int i=0;
	unsigned char x2y2[SM2_NUMWORD*2]={0};
	unsigned char hash[SM2_NUMWORD]={0};
	big C1x,C1y,x2,y2;
	epoint *C1,*S,*dBC1;
	C1x=mirvar(0);
	C1y=mirvar(0);
	x2=mirvar(0);
	y2=mirvar(0);
	C1=epoint_init();
	S=epoint_init();
	dBC1=epoint_init();

	//Step.2 test if C1 fits the curve
	bytes_to_big(SM2_NUMWORD,C,C1x);
	bytes_to_big(SM2_NUMWORD,C+SM2_NUMWORD,C1y);
	epoint_set(C1x,C1y,0,C1);
	i=Test_Point(C1);
	if(i!=0)
	{
		printf("C1 is not on the curve.\n");
		return i;
	}

	//Step.3 S=[h]C1 and test if S is the point at infinity
	ecurve_mult(para_h,C1,S);
	if (point_at_infinity(S))// if S is point at infinity, return error;
	{
		printf("[h]C1 is infinity.\n");
		return ERR_INFINITY_POINT;
	}

	//Step.4 [dB]C1=(x2,y2)
	ecurve_mult(dB,C1,dBC1);
	epoint_get(dBC1,x2,y2);
	big_to_bytes(SM2_NUMWORD,x2,x2y2,1);
	big_to_bytes(SM2_NUMWORD,y2,x2y2+SM2_NUMWORD,1);

	//Step.5 t=KDF(x2||y2,klen)
	SM3_KDF(x2y2,SM2_NUMWORD*2,(Clen-SM2_NUMWORD*3),M);
	if(Test_Null(M,Clen-SM2_NUMWORD*3)!=0)return ERR_ARRAY_NULL;

	//Step.6 M=C2^t
	for(i=0;i<Clen-SM2_NUMWORD*3;i++)
		{
			M[i]=M[i]^C[SM2_NUMWORD*2+i];		
		}

	//Step.7 hash(x2,m,y2)
	SM3_init(&md);
	SM3_process(&md,x2y2,SM2_NUMWORD);
	SM3_process(&md,M,Clen-SM2_NUMWORD*3);
	SM3_process(&md,x2y2+SM2_NUMWORD,SM2_NUMWORD);
	SM3_done(&md,hash);
	if(memcmp(hash,C-SM2_NUMWORD+Clen,SM2_NUMWORD)!=0)
		{
		printf("Summary verification error.\n");
		return ERR_C3_MATCH;
	}
	else
		{
		return 0;		
	}

}

int SM2_RandKey(int index,unsigned char keys[])
{
	//step.1 init
	unsigned char d[64];   
    unsigned char key[64];
    int long_len = 64;
    unsigned char out[long_len/2];
    big a_ks,a_x,a_y;
    epoint *a_kg;
	mip= mirsys(1000, 16);
	mip->IOBASE=16;
	SM2_Init();
    a_ks=mirvar(0);
    a_x=mirvar(0);
    a_y=mirvar(0);
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
	big_to_bytes(SM2_NUMWORD,a_x,kGxy,1);
	big_to_bytes(SM2_NUMWORD,a_y,kGxy+SM2_NUMWORD,1);

	//step.4 output result
	printf("private key:\n");
    for(int i=0;i<SM2_NUMWORD;i++)
    {
		printf("%02x", out[i]);
//		if(i%4==3)printf(" ");
//		if(i%32==31)printf("\n");

    }
    printf("\n");

	printf("public key:\n");
	for(int i=0;i<SM2_NUMWORD*2;i++)
	{
		printf("%02x", kGxy[i]);
//		if(i%4==3)printf(" ");
//		if(i%32==31)printf("\n");
	}
	printf("\n");
	return 0;
}

int ENCRYPT(unsigned char u_message[],unsigned char b_pubkey[])
{
	// unsigned char a_prikey[32];
	// SM2_RandKey(2,a_prikey);
	//step.1 change data to hex bytes.
	int m_len = strlen(u_message);
	unsigned char Message[m_len/2];
	fuchextods(u_message,Message,m_len);
	m_len/=2;

	// unsigned char a_pri[32];
	// int a_len = strlen(a_prikey);
	// fuchextods(a_prikey,a_pri,a_len);
	unsigned char a_pri[32]={
		0x59,0x27,0x6E,0x27,0xD5,0x06,0x86,0x1A,0x16,0x68,0x0F,0x3A,0xD9,0xC0,0x2D,0xCC,
		0xEF,0x3C,0xC1,0xFA,0x3C,0xDB,0xE4,0xCE,0x6D,0x54,0xB8,0x0D,0xEA,0xC1,0xBC,0x21};
	
	unsigned char b_pub[64];
	int b_len = strlen(b_pubkey);
	fuchextods(b_pubkey,b_pub,b_len);

	//step.2 init.
	int tmp=0,i=0;
	unsigned char Cipher[1024];//cipher texy
	unsigned char kGxy[SM2_NUMWORD*2]={0};//SM2_NUMWORD 32
	big ks,x,y;//big number
	epoint *kG;//C1
	mip= mirsys(1000, 16);
	mip->IOBASE=16;
	x=mirvar(0);
	y=mirvar(0);
	ks=mirvar(0);
	kG=epoint_init();
	SM2_Init();	

	//step.3 set b pubkey.
	bytes_to_big(SM2_NUMWORD,b_pub,x);
	bytes_to_big(SM2_NUMWORD,b_pub+SM2_NUMWORD,y);
	epoint_set(x,y,0,kG);

	//step.4 encrypt message.
	tmp=SM2_Encrypt(a_pri,kG,Message,m_len,Cipher);
	if(tmp!=0)
	{
		return tmp;		
	}

	//step.5 output result
	printf("cipher text is :\n");
	for(int i=0;i<m_len+SM2_NUMWORD*3;i++)
	{
		printf("%02x", Cipher[i]);
//		if(i%4==3)printf(" ");
//		if(i%32==31)printf("\n");		
	}
	printf("\n");
	return 0;
}
int DECRYPT(unsigned char Cipher[],unsigned char b_prikey[])
{
	//step.1 change data to hex bytes
	int c_len = strlen(Cipher);
	unsigned char C[c_len/2];//encrypt text
	unsigned char M[c_len/2-SM2_NUMWORD*3];//soure text
	fuchextods(Cipher,C,c_len);

	int Pb_len = strlen(b_prikey);
	unsigned char b_pri[32];
	fuchextods(b_prikey,b_pri,Pb_len);

	//step.2 init
	big ks;
	int flage;
	mip= mirsys(1000, 16);
	mip->IOBASE=16;
	SM2_Init();
	ks=mirvar(0);
	bytes_to_big(SM2_NUMWORD,b_pri,ks);

	//step.3 decrypt message.
	flage=SM2_Decrypt(ks,C,c_len/2,M);
	if(flage!=0)
	{
		return flage;
	}

	//step.4 output result
	printf("M text is :\n");
	for(int i=0;i<c_len/2-SM2_NUMWORD*3;i++)
	{
		printf("%02x", M[i]);
//		if(i%4==3)printf(" ");
//		if(i%32==31)printf("\n");		
	}
	printf("\n");
	return 0;
}
