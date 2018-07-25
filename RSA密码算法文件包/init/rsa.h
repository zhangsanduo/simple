#ifndef __RSA_H__
#define __RSA_H__

#include <stdint.h>

//这是库librsaencrypt.a的头文件


struct public_key_class{
  long long modulus;
  long long exponent;
};

struct private_key_class{
  long long modulus;
  long long exponent;
};
//该函数将加密消息指向的数据。 它返回一个指向堆的指针
//包含加密数据的数组，或失败时为NULL。 这个指针应该在释放时释放
//  加密数据将是原始数据的8倍。
long long *rsa_encrypt(const char *message, const unsigned long message_size, const struct public_key_class *pub);

//该函数将解密消息指向的数据。 它返回一个指向堆的指针
//包含解密数据的数组，或者失败时为NULL。 这个指针应该在释放时释放
// 变量message_size是加密消息的大小（以字节为单位）。
//解密的数据将是加密数据大小的1/8。
char *rsa_decrypt(const long long *message, const unsigned long message_size, const struct private_key_class *pub);

#endif
