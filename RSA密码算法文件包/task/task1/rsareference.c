#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
struct public_key_class{
  long long modulus;
  long long exponent;
};

struct private_key_class{
  long long modulus;
  long long exponent;
};


//这应该完全在数学库中。
long long gcd(long long a, long long b)
{
  long long c;
  while ( a != 0 ) {
    c = a; a = b%a;  b = c;
  }
  return b;
}


long long ExtEuclid(long long a, long long b)
{
 long long x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
 while (a!=0) {
   q = gcd/a; r = gcd % a;
   m = x-u*q; n = y-v*q;
   gcd = a; a = r; x = u; y = v; u = m; v = n;
   }
   return y;
}
//请根据一下注释提示补充下面函数内容
//模函数 C=M^e mod n
long long rsa_modExp(long long b, long long e, long long m)
{


}
//加密函数
long long *rsa_encrypt(const long long *message, const unsigned long message_size,const struct public_key_class *pub)
{

}


char *rsa_decrypt(const long long *message,int len,
                  const struct private_key_class *priv)
{
  long long *temp = malloc(1024);
  // 现在我们遍历每个8字节的块并解密它。
  long long i = 0;
  for(i=0; i < len; i++){
    temp[i] = rsa_modExp(message[i], priv->exponent, priv->modulus);
  }
  return temp;
}

























