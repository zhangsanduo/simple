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



int main(int argc,char *argv[])
{
	if (argc < 2) {
		printf("You must provide at least 1 parameter, where you specify the action.");
		return 1;
	}

  dsd_set_log_level(DSD_DEBUG|DSD_INFO);
  int re=1;
  char  *string = argv[1];

  char result[21];
  char hexresult[41];
  size_t offset;


  /* calculate hash */
  SHA1( result, string, strlen(string) );

  /* format the hash for comparison */
  /*×Ö·û´®16½øÖÆ×ª»»*/
  for( offset = 0; offset < 20; offset++) {
    sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
  }
  
  printf("result=%s\n",hexresult);
  



  return re;
}




