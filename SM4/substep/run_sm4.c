/*************************************************************************
       > File Name: run_sm4.c
       > Author:yangkefeng
       > E-mail:muyidixin2006@126.com
       > Created Time: Thu July 13 23:55:50 2018
************************************************************************/

#include <string.h>
#include <stdio.h>
#include "sm4.h"

int main(int argc, char** argv)
{
	unsigned int mode;
	unsigned char key[16];
	unsigned char input[16];
	
	unsigned char output[16];
	sm4_context ctx;
	unsigned long i;
	
	//SM4 input 读取
	format_park_input(argv[1], input);
	
	//SM4 key 读取
	format_park_input(argv[2], key);
	
	// SM4 mode:encrypt=1/decrypt=0
	mode = atoi(argv[3]);
	
	if(mode == SM4_ENCRYPT)
	{
		//encrypt standard testing vector
		sm4_setkey_enc(&ctx, key);
		sm4_crypt_ecb(&ctx, 1, 16, input, output);
	}
	else
	{
		//decrypt standard testing vector
		sm4_setkey_dec(&ctx, key);
		sm4_crypt_ecb(&ctx, 0, 16, output, output);
	}
	
	//SM4 输出
	for (i = 0; i < 16; i++)
		printf("%02x ", output[i]);
	printf("\n");

	return 0;
}
