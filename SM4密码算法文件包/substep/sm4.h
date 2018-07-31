/*************************************************************************
       > File Name: run_sm4.c
       > Author:yangkefeng
       > E-mail:muyidixin2006@126.com
       > Created Time: Thu July 13 23:55:50 2018
************************************************************************/

#ifndef XYSSL_SM4_H
#define XYSSL_SM4_H

/***
 *sm4 select encrypt/decrypt by flag
 ***/
#define SM4_ENCRYPT     1
#define SM4_DECRYPT     0

/***
 *sm4 select mode by mode
 ***/
#define SM4_ECB 0
#define SM4_CBC 1
#define SM4_CFB 2
#define SM4_OFB 3
#define SM4_CTR 4

/**
 * \brief          SM4 context structure
 */
typedef struct {
    int mode;                   /*!<  encrypt/decrypt   */
    unsigned long sk[32];       /*!<  SM4 subkeys       */
} sm4_context;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          SM4 key schedule (128-bit, encryption)
 *
 * \param ctx      SM4 context to be initialized
 * \param key      16-byte secret key
 */
void sm4_setkey_enc(sm4_context *ctx, unsigned char key[16]);

/**
 * \brief          SM4 key schedule (128-bit, decryption)
 *
 * \param ctx      SM4 context to be initialized
 * \param key      16-byte secret key
 */
void sm4_setkey_dec(sm4_context *ctx, unsigned char key[16]);

/**
 * \brief          SM4-ECB block encryption/decryption
 * \param ctx      SM4 context
 * \param flag     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param input    input block
 * \param output   output block
 */
void sm4_crypt_ecb(sm4_context *ctx,
                   int flag,
                   int length,
                   unsigned char *input,
                   unsigned char *output,int i);

/**
 * \brief          SM4-CBC buffer encryption/decryption
 * \param ctx      SM4 context
 * \param flag     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void sm4_crypt_cbc(sm4_context *ctx,
                   int flag,
                   int length,
                   unsigned char iv[16],
                   unsigned char *input,
                   unsigned char *output);

/**
 * \brief          SM4-CFB encryption/decryption
 * \param ctx      SM4 context
 * \param flag     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void sm4_crypt_cfb(sm4_context *ctx,
                   int flag,
                   int length,
                   unsigned char iv[16],
                   unsigned char *input,
                   unsigned char *output);


/**
 * \brief          SM4-OFB encryption/decryption
 * \param ctx      SM4 context
 * \param flag     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void sm4_crypt_ofb(sm4_context *ctx,
                   int flag,
                   int length,
                   unsigned char iv[16],
                   unsigned char *input,
                   unsigned char *output);


/**
 * \brief          SM4-CTR encryption/decryption
 * \param ctx      SM4 context
 * \param flag     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void sm4_crypt_ctr(sm4_context *ctx,
                   int flag,
                   int length,
                   unsigned char iv[16],
                   unsigned char *input,
                   unsigned char *output);

/**
 * \brief          SM4 encryption/decryption
 * \param key      SM4 key
 * \param flag     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void sm4(unsigned char key[16],
         int mode,
         int flag,
         int length,
         unsigned char iv[16],
         unsigned char *input,
         unsigned char *output);

/**
 * \brief          SM4 parameter format hex
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
int format_park_input(char *input,
                      unsigned char *output);

#ifdef __cplusplus
}
#endif

#endif /* sm4.h */
