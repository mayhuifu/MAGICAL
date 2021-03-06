/*****************************************************************
**
**  Do not modify this file, it is autogenerated by gen_selftest.pl.
**  Your changes will be lost.  You have been warned.
**
*****************************************************************/

/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Bill Chapman

****************************************************************************/
#include <stdio.h>
#include <math.h>

#include "wnlib.h"
#include "wnargp.h"
#include "wnasrt.h"
#include "wnrnd.h"
#include "wnrndd.h"


local void gen_asserts_for_seed_test(void)
{
  int i;

  for(i=0;i<20;++i)
  {
    printf("  wn_assert(wn_random_int() == %d);\n",wn_random_int());
  }  
}


local void test_for_seed_0_result(void)
{
  wn_assert(wn_random_int() == -1650916672);
  wn_assert(wn_random_int() == -344507919);
  wn_assert(wn_random_int() == -515074106);
  wn_assert(wn_random_int() == -724198384);
  wn_assert(wn_random_int() == 1950007689);
  wn_assert(wn_random_int() == -1012613499);
  wn_assert(wn_random_int() == 1959062915);
  wn_assert(wn_random_int() == 218218046);
  wn_assert(wn_random_int() == 642336190);
  wn_assert(wn_random_int() == 1970499599);
  wn_assert(wn_random_int() == -947836837);
  wn_assert(wn_random_int() == -1194637482);
  wn_assert(wn_random_int() == 776699039);
  wn_assert(wn_random_int() == -298801281);
  wn_assert(wn_random_int() == -362539395);
  wn_assert(wn_random_int() == 541129409);
  wn_assert(wn_random_int() == -328106884);
  wn_assert(wn_random_int() == -2056857201);
  wn_assert(wn_random_int() == 1188487311);
  wn_assert(wn_random_int() == 638208991);
}


local void test_for_seed_1_result(void)
{
  wn_assert(wn_random_int() == 751957653);
  wn_assert(wn_random_int() == -1011739186);
  wn_assert(wn_random_int() == 1094111422);
  wn_assert(wn_random_int() == -1245940329);
  wn_assert(wn_random_int() == 910718024);
  wn_assert(wn_random_int() == 593217044);
  wn_assert(wn_random_int() == 1986329062);
  wn_assert(wn_random_int() == -1788010840);
  wn_assert(wn_random_int() == -1138830710);
  wn_assert(wn_random_int() == 2057917689);
  wn_assert(wn_random_int() == -1315651702);
  wn_assert(wn_random_int() == -1697450979);
  wn_assert(wn_random_int() == 1490887820);
  wn_assert(wn_random_int() == 383759670);
  wn_assert(wn_random_int() == -711032777);
  wn_assert(wn_random_int() == -1085266958);
  wn_assert(wn_random_int() == -1492447644);
  wn_assert(wn_random_int() == 984915187);
  wn_assert(wn_random_int() == -1922151855);
  wn_assert(wn_random_int() == 929462588);
}


local void test_for_seed_777777_result(void)
{
  wn_assert(wn_random_int() == 459779487);
  wn_assert(wn_random_int() == -672071191);
  wn_assert(wn_random_int() == 276808002);
  wn_assert(wn_random_int() == 1580750289);
  wn_assert(wn_random_int() == 133907856);
  wn_assert(wn_random_int() == 1847326376);
  wn_assert(wn_random_int() == 1229979287);
  wn_assert(wn_random_int() == -130026417);
  wn_assert(wn_random_int() == -905364977);
  wn_assert(wn_random_int() == 785711089);
  wn_assert(wn_random_int() == 689716002);
  wn_assert(wn_random_int() == 1639142630);
  wn_assert(wn_random_int() == -2127032090);
  wn_assert(wn_random_int() == -1034700081);
  wn_assert(wn_random_int() == -263761699);
  wn_assert(wn_random_int() == 1692250472);
  wn_assert(wn_random_int() == 772398925);
  wn_assert(wn_random_int() == 15681633);
  wn_assert(wn_random_int() == -408972076);
  wn_assert(wn_random_int() == 336386253);
}


local void test_for_seeds(void)
{
  test_for_seed_0_result();

  wn_seed_random_number_generator(0);
  test_for_seed_0_result();

  wn_seed_random_number_generator(1);
  test_for_seed_1_result();

  wn_seed_random_number_generator(777777);
  test_for_seed_777777_result();

  /*
  gen_asserts_for_seed_test();
  */
}


bool wn_s_random_20_bits_false(int trials)
{
  for (  ;  trials > 0;  --trials)
  {
    if (! wn_random_n_bits(20))
    {
      return TRUE;
    }
  }

  return FALSE;
} /* wn_s_random_20_bits_false */


bool wn_s_nth_bit_set_20_times(int trials)
{
  int i, j;

  for (  ;  trials > 0;  --trials)
  {
    i = wn_random_mod_int(32);

    for (j = 0;  j < 20;  ++j)
    {
      if (! (wn_random_int() & (1 << i)))
      {
        break;
      }
    }

    if (j == 20)
    {
      return TRUE;
    }
  }

  return FALSE;
} /* wn_s_nth_bit_set_20_times */


bool wn_s_fail_diag_bits_always_set(int trials)
{
  int i, k;

  i = wn_random_mod_int(32);

  for (  ;  trials > 0;  --trials)
  {
    for (k = 0;  k < 20;  ++k)
    {
      if (!(wn_random_int() & (1 << ((i+k) % 32))))
      {
	break;
      }
    }

    if (20 == k)
    {
      return TRUE;
    }
  }

  return FALSE;
} /* wn_s_fail_diag_bits_always_set */


bool wn_s_two_in_a_row(int trials)
{
  for (  ;  trials > 0;  --trials)
  {
    if (wn_random_n_bits(20) == wn_random_n_bits(20))
    {
      return TRUE;
    }
  }

  return FALSE;
} /* wn_s_two_in_a_row */


int wn_selftest_random(int argc,char *argv[])
{
  int trials, random_seed_arg;
  int sts;

  fprintf(stderr, "testing random stuff...\n");

  test_for_seeds();

  if (argc >= 2)
  {
    sts = sscanf(argv[1], "%d", &random_seed_arg);
    if (1 != sts)
    {
      fprintf(stderr, "Random seed \"%s\" should be an integer\n", argv[1]);
      return 1;
    }

    wn_seed_random_number_generator(random_seed_arg);
  }

  if (argc >= 3)
  {
    sts = sscanf(argv[2], "%d", &trials);
    if (1 != sts)
    {
      fprintf(stderr, "Trials \"%s\" should be an integer\n", argv[2]);
      return 1;
    }
  }
  else
  {
    trials = (int) (log(.9999) / log((1024*1024-1.0)/(1024*1024)));
    /**/			/* comes out to 1/10000 chance of TRUE */
  }

  wn_assert(!wn_s_random_20_bits_false(trials/4));
  wn_assert(!wn_s_nth_bit_set_20_times(trials/4));
  wn_assert(!wn_s_fail_diag_bits_always_set(trials/4));
  wn_assert(!wn_s_two_in_a_row(trials/4));

  if (argc < 3)
  {
    trials = (int) (log(.0001) / log((1024*1024-1.0)/(1024*1024)));
    /**/			/* comes out to 9999/10000 chance of TRUE */
  }

  wn_assert(wn_s_random_20_bits_false(4 * trials));
  wn_assert(wn_s_nth_bit_set_20_times(4 * trials));
  wn_assert(wn_s_fail_diag_bits_always_set(4 * trials));
  wn_assert(wn_s_two_in_a_row(4 * trials));

  fprintf(stderr, "  ok!!!!!!\n");

  return 0;
} /* main */
