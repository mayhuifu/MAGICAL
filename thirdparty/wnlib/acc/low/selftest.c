/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

****************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "wnlib.h"
#include "wnswap.h"
#include "wnasrt.h"
#include "wnabs.h"
#include "wnmax.h"
#include "wnnop.h"


local void selftest_nop(void)
{
  fprintf(stderr,"testing nop....\n");

  wn_do_nothing();
  wn_assert(wn_return_0() == 0);
  wn_assert(wn_return_1() == 1);
  wn_assert(wn_return_neg_1() == -1);
  wn_assert(wn_return_FALSE() == FALSE);
  wn_assert(wn_return_TRUE() == TRUE);

  fprintf(stderr,"  nop ok!!!!!!\n");
}


local void selftest_max(void)
{
  fprintf(stderr,"testing max....\n");

  wn_assert(wn_max(1.5,2.5) == 2.5);
  wn_assert(wn_max(1.5,-2.5) == 1.5);
  wn_assert(wn_max(-1.5,2.5) == 2.5);
  wn_assert(wn_max(-1.5,-2.5) == -1.5);
  wn_assert(wn_max(100,200) == 200);
  wn_assert(wn_max(100,-200) == 100);
  wn_assert(wn_max(-100,200) == 200);
  wn_assert(wn_max(-100,-200) == -100);

  wn_assert(wn_min(1.5,2.5) == 1.5);
  wn_assert(wn_min(1.5,-2.5) == -2.5);
  wn_assert(wn_min(-1.5,2.5) == -1.5);
  wn_assert(wn_min(-1.5,-2.5) == -2.5);
  wn_assert(wn_min(100,200) == 100);
  wn_assert(wn_min(100,-200) == -200);
  wn_assert(wn_min(-100,200) == -100);
  wn_assert(wn_min(-100,-200) == -200);

  fprintf(stderr,"  max ok!!!!!!\n");
}


local void selftest_abs(void)
{
  fprintf(stderr,"testing abs....\n");

  wn_assert(wn_abs(1.5) == 1.5);
  wn_assert(wn_abs(-1.5) == 1.5);
  wn_assert(wn_abs(-100.5e6) == 100.5e6);
  wn_assert(wn_abs(100.5e6) == 100.5e6);
  wn_assert(wn_abs(100) == 100);
  wn_assert(wn_abs(-100) == 100);

  wn_assert(wn_sign(1.5) == 1);
  wn_assert(wn_sign(-1.5) == -1);
  wn_assert(wn_sign(0.0) == 0);
  wn_assert(wn_sign(0) == 0);
  wn_assert(wn_sign(100) == 1);
  wn_assert(wn_sign(-100) == -1);

  fprintf(stderr,"  abs ok!!!!!!\n");
}


local void selftest_swap(void)
{
  fprintf(stderr,"testing swap....\n");

  {
    int i1,i2;

    i1 = 1;
    i2 = 2;

    wn_swap(i1,i2,int);

    wn_assert(i1 == 2);
    wn_assert(i2 == 1);
  }

  {
    const char *s1 = "string1",
	       *s2 = "string2";
    const char *tmp_s1,*tmp_s2;

    tmp_s1 = s1;
    tmp_s2 = s2;

    wn_swap(tmp_s1,tmp_s2,const char *);
  
    wn_assert(tmp_s1 == s2);
    wn_assert(tmp_s2 == s1);
  }

  fprintf(stderr,"  swap ok!!!!!!\n");
}


#if 0
local void selftest_assert()
{
  fprintf(stderr,"testing assert....\n");

  fprintf(stderr,"wn_assert_warn_notreached...\n");
  wn_assert_warn_notreached();

  fprintf(stderr,"wn_assert_warn(1 == 2)...\n");
  wn_assert_warn(1 == 2);

  fprintf(stderr,"wn_assert(1 == 2)...\n");
  wn_assert(1 == 2);
}
#endif


int main(void)
{
  selftest_nop();
  selftest_max();
  selftest_abs();
  selftest_swap();
  /*
  selftest_assert();
  */

  return(0);
}
