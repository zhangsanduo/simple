#include <stdio.h>    
#include <string.h>    
#include <stdlib.h>

#define maxn 0x8000     // 理论支持明文长度    
#define ENCODE 0,16,1       // 加密用的宏    
#define DECODE 15,-1,-1     // 解密用的宏    
#define ACTION_ENCRYPT "-e"
#define ACTION_DECRYPT "-d"    
// 明文初始置换    
char msg_ch[64] = {    
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,    
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,    
    57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,    
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7    
};    
    
// 密钥初始置换    
char key_ch[56] = {    
    57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,    
    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,    
    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,    
    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4    
};    
    
// 扩展置换    
char msg_ex[48] = {    
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,    
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,    
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,    
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1    
};    
    
// 每轮密钥的位移    
char key_mov[16] = {    
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1    
};    
    
// 压缩置换    
char key_cmprs[48] = {    
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,    
    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,    
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,    
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32    
};    
    
// S 盒置换    
char s_box[8][4][16] = {      
    // S1    
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,    
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,    
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,    
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,    
    // S2    
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,    
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,    
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,    
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,    
    // S3    
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,    
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,    
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,    
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,    
    // S4    
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,    
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,    
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,    
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,    
    // S5    
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,    
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,    
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,    
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,    
    // S6    
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,    
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,    
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,    
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,    
    // S7    
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,    
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,    
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,    
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,    
    // S8    
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,    
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,    
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,    
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11    
};    
    
// P 盒置换    
char p_box[32] = {    
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,    
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25    
};    
    
// 末置换    
char last_ch[64] = {    
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,    
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,    
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,    
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25    
};    
    
// hash 置换，将加密后的密文置换为可读明文    
char hs_ch[20] = "0123456789abcdef";    
char sh_ch[128];    
  
void init_trans() {  
    char i;  
    for (i = 0; i < 16; i++)    
        sh_ch[hs_ch[i]] = i;    // 完成hash转换的对应    
}  
    
unsigned char msg[maxn], key[16], res[32],rea[32];    
unsigned char msgb[72], msgbt[72], keyb[18][72];    
    
// 字符转成二进制    
void ChToBit(char* dest, char* src, int length) {    
    int i, j;  
    char t;  
    for (i = 0; i < length; i++) {   
        for (j = 8, t = src[i]; j > 0; j--) {    
            dest[(i << 3) + j] = t & 1;   // 取字符末位    
            t >>= 1;    
        }   
    }  
}    
    
// 二进制转成字符    
void BitToCh(char* dest, char* src, int length) {    
    int i;  
    for (i = 0; i < length << 3; i++) {    
        dest[i >> 3] <<= 1;    
        dest[i >> 3] |= src[i + 1];   // 添加到末位    
    }    
    dest[length] = 0;    
}    
    
// 批置换，以offset为偏移，以count为长度    
void BatchSet(char* dest, char* src, char* offset, int count) {    
    int i;  
    for (i = 0; i < count; i++)    
        dest[i + 1] = src[offset[i]];    
}    
    
// 得到16轮所需的密钥    
void getKeys() {    
    char tk[128], bk[72];    
    char* ptk = tk;    
    int i, j;  
    for (i = 0; i < 8; i++)    
        key[i] <<= 1; // 跳过奇偶校验位    
    ChToBit(bk, key, 8);    
    BatchSet(tk, bk, key_ch, 56);    
    for (i = 0; i < 16; i++) {    
        for (j = 0; j < key_mov[i]; j++, ptk++) {    
            ptk[57] = ptk[28];    
            ptk[28] = ptk[1];    
            // ptk++ 为亮点所在，实质上每一位都没有左移，只是指针右移了    
        }    
        BatchSet(keyb[i], ptk, key_cmprs, 48);    
    }    
}    
    
// 将加密后的密文转换为可读的明文    
void msgPro(char* dest, char* src) {    
    int i, j;    
    for (i = 0; i < 16; i++) {    
        dest[i] = 0;    
        for (j = 1; j <= 4; j++) // 取4位按hash构造一个字符    
            dest[i] = (dest[i] << 1) | src[i * 4 + j];    
        dest[i] = hs_ch[dest[i]];    
    }    
    dest[i] = 0;    
}    
    
// 将密文转换为真正的密文    
void dropMsg(char* dest, char* src) {    
    int i;  
    for (i = 0; i < 16; i++) { // 为hash的逆运算过程    
        dest[i >> 1] = (dest[i >> 1] << 4) | sh_ch[src[i]];    
    }  
}    
    
// 通用加/解密函数，后面三个参数由宏ENCODE和DECODE提供    
void DES(char* pmsg, int st, int cl, int step) {    
    int i, row, col;    
    char r[64], rt[48], s[8];    
    ChToBit(msgbt, pmsg, 8);    
    BatchSet(msgb, msgbt, msg_ch, 64); // 初始置换    
    for (; st != cl; st += step) {    
        memcpy(rt, msgb + 33, 32);    
        BatchSet(r, msgb + 32, msg_ex, 48); // 扩展置换    
        for (i = 1; i <= 48; i++)    
            r[i] ^= keyb[st][i]; // 异或操作    
        // s_box 代替    
        for (i = 0; i < 48; i += 6) {    
            row = col = 0;    
            row = r[i + 1] << 1 | r[i + 6];    
            col = (r[i + 2] << 3) | (r[i + 3] << 2) | (r[i + 4] << 1) | r[i + 5];    
            s[i / 12] = (s[i / 12] <<= 4) | s_box[i / 6][row][col];    
        }    
        ChToBit(r, s, 4);    
        BatchSet(msgb + 32, r, p_box, 32); // p_box 置换    
        for (i = 1; i <= 32; i++)    
            msgb[i + 32] ^= msgb[i]; // 异或    
        memcpy(msgb + 1, rt, 32);    
    }    
    memcpy(msgbt + 33, msgb + 1, 32);    
    memcpy(msgbt + 1, msgb + 33, 32);    
    BatchSet(msgb, msgbt, last_ch, 64); // 末置换    
    if (step == 1)    
        msgPro(res, msgb); // 使密文可读    
    else    
        BitToCh(res, msgb, 8); // 转为原明文    
}    
/*int htoi(char s[])  
{  
    int hexdigit, i, inhex, n;  
  
    i = 0;  
    if(s[i] == '0')         //如果字符串以0x或0X开头，则跳过这两个字符  
    {  
        ++i;  
        if(s[i] == 'x' || s[i] == 'X')    
            ++i;  
    }  
    n = 0;  
    inhex = 1;            //如果字符是有效16进制字符，则置inhex为YES  
    for(; inhex == 1; ++i)  
    {  
        if(s[i] >= '0' && s[i] <= '9')  
            hexdigit = s[i] - '0';  
        else if(s[i] >= 'a' && s[i] <= 'f')  
            hexdigit = s[i] - 'a' + 10;  
        else if(s[i] >= 'A' && s[i] <= 'F')  
            hexdigit = s[i] - 'A' + 10;  
        else  
            inhex = 0;  
        if(inhex == 1)  
            n = n * 16 + hexdigit;  
    }  
  
    return n;  
}  */
char valueToHexCh(const int value)
{
  char result = '\0';
  if(value >= 0 && value <= 9){
    result = (char)(value + 48); //48为ascii编码的‘0’字符编码值
  }
  else if(value >= 10 && value <= 15){
    result = (char)(value - 10 + 65); //减去10则找出其在16进制的偏移量，65为ascii的'A'的字符编码值
  }
  else{
    ;
  }

  return result;
}

int strToHex(char *ch, char *hex)
{
  int high,low;
  int tmp = 0;
  if(ch == NULL || hex == NULL){
    return -1;
  }

  if(strlen(ch) == 0){
    return -2;
  }

  while(*ch){
    tmp = (int)*ch;
    high = tmp >> 4;
    low = tmp & 15;
    *hex++ = valueToHexCh(high); //先写高字节
    *hex++ = valueToHexCh(low); //其次写低字节
    ch++;
  }
  *hex = '\0';
  return 0;
} 
unsigned char CharToHex(unsigned char bHex)  
{  
    if((bHex>=0)&&(bHex<=9))  
    {  
        bHex += 0x30;  
    }  
    else if((bHex>=10)&&(bHex<=15))//Capital  
    {  
        bHex += 0x37;  
    }  
    else   
    {  
        bHex = 0xff;  
    }  
    return bHex;  
}
/* 本程序为其他程序调用，所有参数自己定义  
 * 详细参数：  
 * des d/e key msg  
 *  -d——解密   -e——加密  
 *  key——密钥  
 *  msg——要加/解密的明/密文  
 */    
int main(int argc, char* argv[]) {    
    if (argc < 3) {    
        printf("Input Error");    
        return 0;    
    }    
    init_trans();  
    //char * rea;
    
    // 读取参数过程    
  //  char mode = arv[1][0];    
    strcpy(key, argv[2]);    
    strcpy(msg, argv[3]);    
    
    getKeys(); // 得到16轮要用到的密钥    
    
    int i;  long j;char * string;char hex[1024];
    if (strcmp(argv[1], ACTION_ENCRYPT) == 0) {    
        for (i = 0; msg[i]; i += 8) {    
            DES(msg + i, ENCODE); // 加密    
            printf("%s", res);    
        }    
    } else if (strcmp(argv[1], ACTION_DECRYPT) == 0) {    
        for (i = 0; msg[i]; i += 16) {    
            dropMsg(res, msg + i); // 将密文转换为真正的密文    
            DES(res, DECODE); // 解密  
          /* int rea=0;
           rea = htoi(res);
           printf("%d\n", rea);*/
          /*  msgPro(rea,msg+i);
            printf("%s\n", rea);*/
           // printf("%lx\n",strtol (res,&string,16)); 
           /* strToHex(res,hex);  
            printf("%s\n", hex); */
            for(int i=0;i<strlen(res);i++)
                 printf("%x",(unsigned int)res[i]);
        }    
    } else {    
        printf("Input Error!!!");    
    }    
    printf("\n");  
    return 0;    
}  
