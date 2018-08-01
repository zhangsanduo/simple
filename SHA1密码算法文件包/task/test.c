/*
SHA1 tests by Philip Woolford <woolford.philip@gmail.com>
100% Public Domain
 */

#include "sha1.h"
#include "stdio.h"
#include "string.h"
#include "debug.h"

#define SUCCESS 0

/* The suite initialization function.
 * Returns zero on success, non-zero otherwise.
 
 */
int init_suite(
    void
)
{
  return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite(
    void
)
{
  return 0;
}

/* Test Vector 1 */
int main(
    void
)
{

  dsd_set_log_level(DSD_DEBUG|DSD_INFO);
  int re;
  char const string[] = "abc";
  char const expect[] = "a9993e364706816aba3e25717850c26c9cd0d89d";
  char result[21];
  char hexresult[41];
  size_t offset;
/*  printf("start");
  printf("result= %s", $expect);
*/

  /* calculate hash */
  SHA1( result, string, strlen(string) );

  dsdl_info("result=%s\n",result);
  /* format the hash for comparison */
  /*×Ö·û´®16½øÖÆ×ª»»*/
  for( offset = 0; offset < 20; offset++) {
    sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
  }
  
  printf("result=%s\n",hexresult);
  
  re = strncmp(hexresult, expect, 40); 
  
  return re;

}




