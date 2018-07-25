/*************************************************************************
       > File Name: run_sm4.c
       > Author:yangkefeng
       > E-mail:muyidixin2006@126.com
       > Created Time: Thu July 13 23:55:50 2018
************************************************************************/

#include <string.h>
#include <stdio.h>

#include "sm4.h"

int main(int argc, char **argv) {
    int mode = 0, flag = 1;
    unsigned char key[16];
    unsigned char iv[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                            0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    unsigned char input[16];

    unsigned char output[16];
    sm4_context ctx;
    long i;

    //SM4 input 读取
    format_park_input(argv[1], input);

    //SM4 key 读取
    format_park_input(argv[2], key);

    // SM4 mode:SM4_ECB 0,SM4_CBC 1,SM4_CFB 2,SM4_OFB 3,SM4_CTR 4
    mode = atoi(argv[3]);
    printf("mode:%d\n", mode);
    // SM4 flag:encrypt=1/decrypt=0
    flag = atoi(argv[4]);
    printf("flag:%d\n", flag);

    sm4(key, mode, flag, 16, iv, input, output);

    //SM4 输出
    for (i = 0; i < 16; i++)
        printf("%02x ", output[i]);
    printf("\n");

    return 0;
}
