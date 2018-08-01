/*
SHA-1 in C
By Steve Reid <steve@edmweb.com>
100% Public Domain

Test Vectors (from FIPS PUB 180-1)
"abc"
  A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
  84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
A million repetitions of "a"
  34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

/* #define LITTLE_ENDIAN * This should be #define'd already, if true. */
/* #define SHA1HANDSOFF * Copies data before messing with it. */

#define SHA1HANDSOFF

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* for uint32_t */
#include <stdint.h>

#include "sha1.h"
#include "debug.h"


#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#if BYTE_ORDER == LITTLE_ENDIAN
#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
    |(rol(block->l[i],8)&0x00FF00FF))
#elif BYTE_ORDER == BIG_ENDIAN
#define blk0(i) block->l[i]
#else
#error "Endianness not defined!"
#endif
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) printf("sha1_3_1_1_%d:%x\n",i,v);\
		printf("sha1_3_1_2_%d:%x\n",i,w);\
		printf("sha1_3_1_3_%d:%x\n",i,x);\
		printf("sha1_3_1_4_%d:%x\n",i,y);\
		printf("sha1_3_1_5_%d:%x\n",i,z);\
		printf("sha1_3_1_6_%d:%x\n",i,(rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF));\
		printf("sha1_3_1_7_%d:5A827999\n",i);\
		printf("sha1_3_2_1_%d:%x\n",i,rol(v,5));\
	    printf("sha1_3_3_1_%d:%x\n",i,rol(w,30));\
		printf("sha1_3_5_1_%d:%x\n",i,(((w&(x^y))^y)+z));\
		printf("sha1_3_6_1_%d:%x\n",i,(((w&(x^y))^y)+z+rol(v,5)));\
		printf("sha1_2_4_%d_%d:%08x\n",i+1,i,(rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF));\
		printf("sha1_3_7_1_%d:%x\n",i,(((w&(x^y))^y)+z+rol(v,5)+((rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF))));\
	    z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);\
		printf("sha1_3_4_1_%d:%x\n",i,(((w&(x^y))^y)));w=rol(w,30);\
		printf("sha1_3_8_1_%d:%x\n",i, z);\
		printf("sha1_3_8_2_%d:%x\n",i, v);\
		printf("sha1_3_8_3_%d:%x\n",i, x);\
		printf("sha1_3_8_4_%d:%x\n",i, y);


	

#define R1(v,w,x,y,z,i,str) printf("sha1_3_1_1_%d:%x\n",i,v);\
		printf("sha1_3_1_2_%d:%x\n",i,w);\
		printf("sha1_3_1_3_%d:%x\n",i,x);\
		printf("sha1_3_1_4_%d:%x\n",i,y);\
		printf("sha1_3_1_5_%d:%x\n",i,z);\
		printf("sha1_3_1_6_%d:%x\n",i,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		printf("sha1_3_1_7_%d:5A827999\n",i);\
		printf("sha1_3_2_1_%d:%x\n",i,rol(v,5));\
	    printf("sha1_3_3_1_%d:%x\n",i,rol(w,30));\
		printf("sha1_3_5_1_%d:%x\n",i,(((w&(x^y))^y)+z));\
		printf("sha1_3_6_1_%d:%x\n",i,(((w&(x^y))^y)+z+rol(v,5)));\
		printf("sha1_2_4_%d_%d:%08x\n",i+1,i,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		sprintf(str, "%s%08x", str,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)) );\
		printf("sha1_3_7_1_%d:%x\n",i,(((w&(x^y))^y)+z+rol(v,5)+(rol(block->l[(i+13)&15]^block->l[(i+8)&15]^block->l[(i+2)&15]^block->l[i&15],1))));\
		z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);\
		printf("sha1_3_4_1_%d:%x\n",i,(((w&(x^y))^y)));w=rol(w,30);\
		printf("sha1_3_8_1_%d:%x\n",i, z);\
		printf("sha1_3_8_2_%d:%x\n",i, v);\
		printf("sha1_3_8_3_%d:%x\n",i, x);\
		printf("sha1_3_8_4_%d:%x\n",i, y);
		


#define R2(v,w,x,y,z,i,str) printf("sha1_3_1_1_%d:%x\n",i,v);\
		printf("sha1_3_1_2_%d:%x\n",i,w);\
		printf("sha1_3_1_3_%d:%x\n",i,x);\
		printf("sha1_3_1_4_%d:%x\n",i,y);\
		printf("sha1_3_1_5_%d:%x\n",i,z);\
		printf("sha1_3_1_6_%d:%x\n",i,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		printf("sha1_3_1_7_%d:6ED9EBA1\n",i);\
		printf("sha1_3_2_1_%d:%x\n",i,rol(v,5));\
	    printf("sha1_3_3_1_%d:%x\n",i,rol(w,30));\
		printf("sha1_3_5_1_%d:%x\n",i,((w^x^y)+z));\
		printf("sha1_3_6_1_%d:%x\n",i,((w^x^y)+z+rol(v,5)));\
		printf("sha1_2_4_%d_%d:%08x\n",i+1,i,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		sprintf(str, "%s%08x", str,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)) );\
		printf("sha1_3_7_1_%d:%x\n",i,((w^x^y)+z+rol(v,5)+rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);\
		printf("sha1_3_4_1_%d:%x\n",i,((w^x^y)));w=rol(w,30);\
		printf("sha1_3_8_1_%d:%x\n",i, z);\
		printf("sha1_3_8_2_%d:%x\n",i, v);\
		printf("sha1_3_8_3_%d:%x\n",i, x);\
		printf("sha1_3_8_4_%d:%x\n",i, y);
		
#define R3(v,w,x,y,z,i,str)  printf("sha1_3_1_1_%d:%x\n",i,v);\
		printf("sha1_3_1_2_%d:%x\n",i,w);\
		printf("sha1_3_1_3_%d:%x\n",i,x);\
		printf("sha1_3_1_4_%d:%x\n",i,y);\
		printf("sha1_3_1_5_%d:%x\n",i,z);\
		printf("sha1_3_1_6_%d:%x\n",i,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		printf("sha1_3_1_7_%d:8F1BBCDC\n",i);\
		printf("sha1_3_2_1_%d:%x\n",i,rol(v,5));\
	    printf("sha1_3_3_1_%d:%x\n",i,rol(w,30));\
		printf("sha1_3_5_1_%d:%x\n",i,((((w|x)&y)|(w&x))+z));\
		printf("sha1_3_6_1_%d:%x\n",i,((((w|x)&y)|(w&x))+z+rol(v,5)));\
		printf("sha1_2_4_%d_1:%08x\n",i+1,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		sprintf(str, "%s%08x", str,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)) );\
		printf("sha1_3_7_1_%d:%x\n",i,((((w|x)&y)|(w&x))+z+rol(v,5)+rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);\
		printf("sha1_3_4_1_%d:%x\n",i,(((w|x)&y)|(w&x)));w=rol(w,30);\
		printf("sha1_3_8_1_%d:%x\n",i, z);\
		printf("sha1_3_8_2_%d:%x\n",i, v);\
		printf("sha1_3_8_3_%d:%x\n",i, x);\
		printf("sha1_3_8_4_%d:%x\n",i, y);
		


#define R4(v,w,x,y,z,i,str) printf("sha1_3_1_1_%d:%x\n",i,v);\
		printf("sha1_3_1_2_%d:%x\n",i,w);\
		printf("sha1_3_1_3_%d:%x\n",i,x);\
		printf("sha1_3_1_4_%d:%x\n",i,y);\
		printf("sha1_3_1_5_%d:%x\n",i,z);\
		printf("sha1_3_1_6_%d:%x\n",i,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		printf("sha1_3_1_7_%d:CA62C1D6\n",i);\
		printf("sha1_3_2_1_%d:%x\n",i,rol(v,5));\
	    printf("sha1_3_3_1_%d:%x\n",i,rol(w,30));\
		printf("sha1_3_5_1_%d:%x\n",i,((w^x^y)+z));\
		printf("sha1_3_6_1_%d:%x\n",i,((w^x^y)+z+rol(v,5)));\
		printf("sha1_2_4_%d_1:%08x\n",i+1,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		sprintf(str, "%s%08x", str,(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)) );\
		printf("sha1_3_7_1_%d:%x\n",i,((w^x^y)+z+rol(v,5)+rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));\
		z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);\
		printf("sha1_3_4_1_%d:%x\n",i,((w^x^y)));w=rol(w,30);\
		printf("sha1_3_8_1_%d:%x\n",i, z);\
		printf("sha1_3_8_2_%d:%x\n",i, v);\
		printf("sha1_3_8_3_%d:%x\n",i, x);\
		printf("sha1_3_8_4_%d:%x\n",i, y);
		


 
void hextobin(char *str)
{ 
	
		int n; 
 		sscanf(str,"%x",&n);
	
        int a[8];
        int i;
        for (i = 0; i != 8; ++i)
        {
                a[8 - 1 - i] = n % 2;
                n /= 2;
        }
        for (i = 0; i != 8; ++i)
        {
                printf("%d",a[i]);
        }

		/*printf("\n");*/
}

 


 
/* Hash a single 512-bit block. This is the core of the algorithm. */

void SHA1Transform(
    uint32_t state[5],
    const unsigned char buffer[64]
)
{
	int j;
    int i;
	char str[4096];
    uint32_t a, b, c, d, e;

    typedef union
    {
        unsigned char c[64];
        uint32_t l[16];
    } CHAR64LONG16;

#ifdef SHA1HANDSOFF
    CHAR64LONG16 block[1];      /* use array to appear as a pointer */

    memcpy(block, buffer, 64);
#else
    /* The following had better never be used because it causes the
     * pointer-to-const buffer to be cast into a pointer to non-const.
     * And the result is written through.  I threw a "const" in, hoping
     * this will cause a diagnostic.
     */
    CHAR64LONG16 *block = (const CHAR64LONG16 *) buffer;
#endif
    
    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
	/*Ìî³äµ½512Î»*/
	printf("sha1_1_3_1_1:");
	for (i=0; i<64; i++)
	  printf("%x",buffer[i]);
	printf("\n");

	for (i=0,j=1; i<5; i++,j++)
	  printf("sha1_1_4_%d_1:%x\n",j,state[i]);
	  /*dsdl_info("0x%x\n",state[i]);*/
	  
	printf("sha1_1_5_1_1:");
	for (i=0;i<4;i++)
		printf("%08x",(rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF));  
	printf("\n");	
	
	
	printf("sha1_1_5_2_1:");
	for (i=4;i<8;i++)
		printf("%08x",(rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF));  
	printf("\n");
	
	printf("sha1_1_5_3_1:");
	for (i=8;i<12;i++)
		printf("%08x",(rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF));  
	printf("\n");	
	
	printf("sha1_1_5_4_1:");
	for (i=12;i<16;i++)
		printf("%08x",(rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF));  
	printf("\n");	



	for (i=0,j=1; i<5; i++,j++)
	  printf("sha1_1_7_%d_1:%x\n",j,state[i]);
	  /*dsdl_info("0x%x\n",state[i]);*/
	
	
    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a, b, c, d, e, 0);
    R0(e, a, b, c, d, 1);
    R0(d, e, a, b, c, 2);
    R0(c, d, e, a, b, 3);
    R0(b, c, d, e, a, 4);
    R0(a, b, c, d, e, 5);
    R0(e, a, b, c, d, 6);
    R0(d, e, a, b, c, 7);
    R0(c, d, e, a, b, 8);
    R0(b, c, d, e, a, 9);
    R0(a, b, c, d, e, 10);
    R0(e, a, b, c, d, 11);
    R0(d, e, a, b, c, 12);
    R0(c, d, e, a, b, 13);
    R0(b, c, d, e, a, 14);
    R0(a, b, c, d, e, 15);
/*	
	printf("sha1_1_6_1_1:");
	for (i=16;i<79;i++)
		printf("%x",(rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1)));
	printf("\n");
	*/
    R1(e, a, b, c, d, 16,str);
    R1(d, e, a, b, c, 17,str);
    R1(c, d, e, a, b, 18,str);
    R1(b, c, d, e, a, 19,str);
    R2(a, b, c, d, e, 20,str);
    R2(e, a, b, c, d, 21,str);
    R2(d, e, a, b, c, 22,str);
    R2(c, d, e, a, b, 23,str);
    R2(b, c, d, e, a, 24,str);
    R2(a, b, c, d, e, 25,str);
    R2(e, a, b, c, d, 26,str);
    R2(d, e, a, b, c, 27,str);
    R2(c, d, e, a, b, 28,str);
    R2(b, c, d, e, a, 29,str);
    R2(a, b, c, d, e, 30,str);
    R2(e, a, b, c, d, 31,str);
    R2(d, e, a, b, c, 32,str);
    R2(c, d, e, a, b, 33,str);
    R2(b, c, d, e, a, 34,str);
    R2(a, b, c, d, e, 35,str);
    R2(e, a, b, c, d, 36,str);
    R2(d, e, a, b, c, 37,str);
    R2(c, d, e, a, b, 38,str);
    R2(b, c, d, e, a, 39,str);
    R3(a, b, c, d, e, 40,str);
    R3(e, a, b, c, d, 41,str);
    R3(d, e, a, b, c, 42,str);
    R3(c, d, e, a, b, 43,str);
    R3(b, c, d, e, a, 44,str);
    R3(a, b, c, d, e, 45,str);
    R3(e, a, b, c, d, 46,str);
    R3(d, e, a, b, c, 47,str);
    R3(c, d, e, a, b, 48,str);
    R3(b, c, d, e, a, 49,str);
    R3(a, b, c, d, e, 50,str);
    R3(e, a, b, c, d, 51,str);
    R3(d, e, a, b, c, 52,str);
    R3(c, d, e, a, b, 53,str);
    R3(b, c, d, e, a, 54,str);
    R3(a, b, c, d, e, 55,str);
    R3(e, a, b, c, d, 56,str);
    R3(d, e, a, b, c, 57,str);
    R3(c, d, e, a, b, 58,str);
    R3(b, c, d, e, a, 59,str);
    R4(a, b, c, d, e, 60,str);
    R4(e, a, b, c, d, 61,str);
    R4(d, e, a, b, c, 62,str);
    R4(c, d, e, a, b, 63,str);
    R4(b, c, d, e, a, 64,str);
    R4(a, b, c, d, e, 65,str);
    R4(e, a, b, c, d, 66,str);
    R4(d, e, a, b, c, 67,str);
    R4(c, d, e, a, b, 68,str);
    R4(b, c, d, e, a, 69,str);
    R4(a, b, c, d, e, 70,str);
    R4(e, a, b, c, d, 71,str);
    R4(d, e, a, b, c, 72,str);
    R4(c, d, e, a, b, 73,str);
    R4(b, c, d, e, a, 74,str);
    R4(a, b, c, d, e, 75,str);
    R4(e, a, b, c, d, 76,str);
    R4(d, e, a, b, c, 77,str);
    R4(c, d, e, a, b, 78,str);
    R4(b, c, d, e, a, 79,str);


	dsdl_info("80 step\n");
	printf("sha1_1_8_1_1:%x\n",a);
	printf("sha1_1_8_2_1:%x\n",b);
	printf("sha1_1_8_3_1:%x\n",c);
	printf("sha1_1_8_4_1:%x\n",d);
	printf("sha1_1_8_5_1:%x\n",e);

    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
	dsdl_info("sum\n");

	for (i=0,j=1; i<5; i++,j++)
	  printf("sha1_1_9_%d_1:%x\n",j,state[i]);

	for (i=0,j=9; i<5; i++,j++)
	  printf("sha1_2_%d_1_1:%x\n",j,state[i]);

	printf("sha1_1_6_1_1:%s\n",str);
    /* Wipe variables */
    a = b = c = d = e = 0;
#ifdef SHA1HANDSOFF
    memset(block, '\0', sizeof(block));
#endif
}


/* SHA1Init - Initialize new context */

void SHA1Init(
    SHA1_CTX * context
)
{
    /* SHA1 initialization constants */
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
}


/* Run your data through this. */

void SHA1Update(
    SHA1_CTX * context,
    const unsigned char *data,
    uint32_t len
)
{
    uint32_t i;

    uint32_t j;

    j = context->count[0];
    if ((context->count[0] += len << 3) < j)
        context->count[1]++;
    context->count[1] += (len >> 29);
    j = (j >> 3) & 63;
    if ((j + len) > 63)
    {
        memcpy(&context->buffer[j], data, (i = 64 - j));
        SHA1Transform(context->state, context->buffer);
        for (; i + 63 < len; i += 64)
        {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0;
    }
    else
        i = 0;
    memcpy(&context->buffer[j], &data[i], len - i);
}


/* Add padding and return the message digest. */

void SHA1Final(
    unsigned char digest[20],
    SHA1_CTX * context
)
{
    unsigned i;


    unsigned char finalcount[8];

    unsigned char c;

#if 0    /* untested "improvement" by DHR */
    /* Convert context->count to a sequence of bytes
     * in finalcount.  Second element first, but
     * big-endian order within element.
     * But we do it all backwards.
     */
    unsigned char *fcp = &finalcount[8];

    for (i = 0; i < 2; i++)
    {
        uint32_t t = context->count[i];

        int j;

        for (j = 0; j < 4; t >>= 8, j++)
            *--fcp = (unsigned char) t}
#else
    for (i = 0; i < 8; i++)
    {
        finalcount[i] = (unsigned char) ((context->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);      /* Endian independent */
    }
#endif
    c = 0200;
	printf("\n");
	for (i=0; i<64; i++)
	{

		if(0==context->buffer[i])
			break;
		printf("%x",context->buffer[i]);	
	}
	printf("\n");
	
    SHA1Update(context, &c, 1);

	printf("sha1_1_1_1_1:");
	for (i=0; i<64;i++)
		{
/*¶þ½øÖÆÌî³ä1*/
		char str[80];
		char str1[80] = "0x80";
		sprintf(str, "0x%x",context->buffer[i]);
		hextobin(str);
		if(strcmp(str,str1)==0)

			break;
		}

	printf("\n");
	
    while ((context->count[0] & 504) != 448)
    {
        c = 0000;
        SHA1Update(context, &c, 1);
    }
/*Ìî³ä0µ½448Î»*/
	printf("sha1_1_2_1_1:");
	for (i=0; i<56; i++)
		printf("%x",context->buffer[i]);
	
	printf("\n");
	
    SHA1Update(context, finalcount, 8); /* Should cause a SHA1Transform() */
    for (i = 0; i < 20; i++)
    {
        digest[i] = (unsigned char)
            ((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
    }
    /* Wipe variables */
    memset(context, '\0', sizeof(*context));
    memset(&finalcount, '\0', sizeof(finalcount));
}

void SHA1(
    char *hash_out,
    const char *str,
    int len)
{
    SHA1_CTX ctx;
    unsigned int ii;
	unsigned int i;
		
    SHA1Init(&ctx);
	dsdl_info("init..............\n");
	for (i=0; i<5; i++)
		dsdl_info("H[%d]0x%x\n",i,ctx.state[i]);
	for (i=0;i<2;i++)
		dsdl_info("count[%d]0x%x\n",i,ctx.count[i]);
	
	/*for (i=0; i<64; i++)
		printf("%d %x\n",i,ctx.buffer[i]);*/	
	
	dsdl_info("init..........end....\n\n\n");

	
    for (ii=0; ii<len; ii+=1)
        SHA1Update(&ctx, (const unsigned char*)str + ii, 1);

	
	dsdl_info("new..............\n\n\n");
	for (i=0; i<5; i++)
		dsdl_info("H[%d]0x%x\n",i,ctx.state[i]);
	for (i=0;i<2;i++)
		printf("count[%d]0x%x\n",i,ctx.count[i]);
	
	/*for (i=0; i<64; i++)
		//printf("buffer[%d]0x%x",i,ctx.buffer[i]);
		printf("%d  %x\n",i, ctx.buffer[i]);*/
	
	printf("\n new..........end....\n\n");
	
    SHA1Final((unsigned char *)hash_out, &ctx);
    hash_out[20] = '\0';
}

