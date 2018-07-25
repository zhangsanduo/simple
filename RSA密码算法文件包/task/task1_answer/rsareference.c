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

long long rsa_modExp(long long b, long long e, long long m)
{
  if (b < 0 || e < 0 || m <= 0){
    exit(1);
  }
  b = b % m;
  if(e == 0) return 1;
  if(e == 1) return b;
  if( e % 2 == 0){
    return ( rsa_modExp(b * b % m, e/2, m) % m );
  }
  if( e % 2 == 1){
    return ( b * rsa_modExp(b, (e-1), m) % m );
  }

}

long long *rsa_encrypt(const long long *message, const unsigned long message_size,const struct public_key_class *pub)
{
  long long *encrypted = malloc(sizeof(long long)*message_size);
  if(encrypted == NULL){
    fprintf(stderr,
     "Error: Heap allocation failed.\n");
    return NULL;
  }
  long long i = 0;
  for(i=0; i < message_size; i++){
    encrypted[i] = rsa_modExp(message[i], pub->exponent, pub->modulus);
  }
  return encrypted;
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

























