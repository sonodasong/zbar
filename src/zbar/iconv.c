#include <iconv.h>
#include <errno.h>
#include <stdio.h>

size_t iconv(iconv_t cd __attribute__((unused)),
             char **inbuf, size_t *inbytesleft,
             char **outbuf, size_t *outbytesleft)
{
  /* The main case is when inbuf is not NULL and *inbuf is not NULL */
  if(inbuf && *inbuf) {
    size_t bytesConverted = 0;

    if(!outbuf || !(*outbuf)) {
      /* this case is not noted by the man page, which is sad */
      printf("bad iconv() case; (*)inbuf is good but (*)outbuf isn't\n");
      return -1;
    }

    do {
      if(*inbytesleft == 0)
        return bytesConverted;
      if(*outbytesleft == 0) {
        return -1;
      }

      **outbuf       = **inbuf;
      *outbuf       += 1;
      *inbuf        += 1;
      *outbytesleft -= 1;
      *inbytesleft  -= 1;
    } while(1);
  }

  /* the other two cases appear to not apply, or the appropriate return */
  /* value is zero in any case.                                         */
  return 0;
}

int iconv_close(iconv_t cd __attribute__((unused)))
{
  return 0;
}

/* GHC apparantly can't get along without iconv. So this is the world's */
/* most unimpressive implementation of it. There is only one iconv_t    */
/* return value (0xacce55ed), and it always translates things exactly   */
/* byte for byte, no matter what you do.                                */
iconv_t iconv_open(const char *tocode __attribute__((unused)),
                   const char *fromcode __attribute__((unused)))
{
  return (iconv_t)0xacce55ed;
}
