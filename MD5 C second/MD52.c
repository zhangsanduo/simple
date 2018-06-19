#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
//<stdint.h> 定义了 int16_t 、 uint32_t 、 int64_t 等整型
// 元素表T（常量值）
const uint32_t k[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

// 四轮移动量
const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

// 左旋函数定义
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void to_bytes(uint32_t val, uint8_t *bytes)//转8位
{
    bytes[0] = (uint8_t) val;//取32位的前8位
    bytes[1] = (uint8_t) (val >> 8);//取32位的第2个8位
    bytes[2] = (uint8_t) (val >> 16);
    bytes[3] = (uint8_t) (val >> 24);
}

uint32_t to_int32(const uint8_t *bytes)//转32位
{
    return (uint32_t) bytes[0]
        | ((uint32_t) bytes[1] << 8)
        | ((uint32_t) bytes[2] << 16)
        | ((uint32_t) bytes[3] << 24);
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
void fuchextods(char intput[],char output[],int len)//exz转ASCII
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
void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest)//MD5加密，参数依次为明文，明文长度，加密结果
 {

    // 这些变量将包含散列
    uint32_t h0, h1, h2, h3;
    //消息
    uint8_t *msg = NULL;
    size_t new_len, offset;
    uint32_t w[16];
    uint32_t a, b, c, d, i, f, g, temp;

    // 初始化变量
    h0 = 0x67452301;//A
    h1 = 0xefcdab89;//B
    h2 = 0x98badcfe;//C
    h3 = 0x10325476;//D

   //预处理：
     //将“1”位附加到消息
     //追加“0”位，直到位≡448中的消息长度（模512）
     //添加长度mod（2 ^ 64）到消息
    for (new_len = initial_len + 1; new_len % (512/8) != 448/8; new_len++)
        ;

    msg = (uint8_t*)malloc(new_len + 8);
    memcpy(msg, initial_msg, initial_len);
    //消息填充
    msg[initial_len] = 0x80; //追加“1”位
    for (offset = initial_len + 1; offset < new_len; offset++)
        msg[offset] = 0; // 追加 "0" 位

    //在缓冲区末尾添加len位。（补足长度）
    to_bytes(initial_len*8, msg + new_len);
    // initial_len >> 29 == initial_len * 8 >> 32，但避免溢出
    to_bytes(initial_len>>29, msg + new_len + 4);
    //以连续的512位块处理消息：
    //对于每个512位的消息块：
    for(offset=0; offset<new_len; offset += (512/8))
    {
        //将块分成16个32位字w [j]，0≤j≤15
        for (i = 0; i < 16; i++)
            w[i] = to_int32(msg + offset + i*4);
    //为这个块初始化散列值：
        a = //h0;
        b = //h1;
        c =// h2;
        d = //h3;
        // 主要循环:
        for(i = 0; i<64; i++)
        {

            if (i < 16)
            {
                f = //(b & c) | ((~b) & d);//基本逻辑函数F
                g = //i;//第一轮依次加1
            }
            else if (i < 32)
            {
                f = //(d & b) | ((~d) & c);//基本逻辑函数G
                g = //(5*i + 1) % 16;//第二轮依次加5模16
            }
             else if (i < 48)
             {
                f =// b ^ c ^ d;//基本逻辑函数H
                g = //(3*i + 5) % 16;//第三轮依次加3模16
            }
            else
            {
                f =// c ^ (b | (~d));//基本逻辑函数I
                g = //(7*i) % 16;//第四轮依次加7模16
            }

            //temp = d;
            d = //c;
            c = //b;
            b = //b + LEFTROTATE ((a + f + k[i] + w[g]), r[i]);
            a = //temp;

        }

        // 添加此块的散列到目前为止：
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;

    }

    // cleanup
    free(msg);

    // var unit_8 digest [16] 将结果放入digest 形成最后128it
    to_bytes(h0, digest);
    to_bytes(h1, digest + 4);
    to_bytes(h2, digest + 8);
    to_bytes(h3, digest + 12);
}

int main(int argc, char **argv)
{
    char *msg;//消息
    size_t len;
    uint8_t result[16];
    char intput[512];//16进制输入
    char output[256];//字符输出
    int i;
    //scanf("%s",&intput);
     if(!strcmp(argv[1], "--hex")|| !strcmp(argv[1], "-h"))
    {
         strcpy(intput, argv[2]);
         len=strlen(intput);
        if(len%2!=0)//判断是否为双数位
        {
            printf("D41D8CD98F00B204E9800998ECF8427E\n");
            return 0;
        }
         if(hextest(intput,len)==1)
        {
            fuchextods(intput,&output,len);
            msg= (char *)malloc(strlen(output));
            strcpy(msg,output);//将转好字符放入msg后进行加密
        }
        else
        {
            printf("16进制不符合要求");
            return 0;
        }

    }
    if(!strcmp(argv[1], "--char")|| !strcmp(argv[1], "-c"))
    {
        msg=argv[2];
    }
    len = strlen(msg);
     //基准
     for (i = 0; i < 1000000; i++)
            md5((uint8_t*)msg, len, result);//MD5计算
    // 输出结果
    for (i = 0; i < 16; i++)
        printf("%2.2x", result[i]);
    puts("");
    return 0;
}
