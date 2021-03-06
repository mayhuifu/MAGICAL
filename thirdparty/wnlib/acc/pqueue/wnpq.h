/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Will Naylor

****************************************************************************/
#ifndef wnpqH
#define wnpqH


#include "wnlib.h"
#include "wnmem.h"
#include "wnpary.h"


typedef struct wn_pqueue_struct *wn_pqueue;
typedef struct wn_pqhandle_struct *wn_pqhandle;

struct wn_pqueue_struct
{
  int num_used,num_alloced;
  wn_pqhandle *handle_array;

  wn_pqhandle free_handle_list;

  wn_memgp group;
};

struct wn_pqhandle_struct
{
  double key;
  int index;

  ptr contents;
};
  

EXTERN void wn_mkpqueue(wn_pqueue *pqueue);
EXTERN void wn_freepqueue(wn_pqueue queue);

EXTERN void wn_pqgetmin(wn_pqhandle *phandle,wn_pqueue queue);
EXTERN void wn_pqins(wn_pqhandle *phandle,wn_pqueue queue,double key);
EXTERN void wn_pqdel(wn_pqhandle handle,wn_pqueue queue);
EXTERN void wn_pqmove(wn_pqhandle handle,wn_pqueue queue,double new_key);

EXTERN int wn_pqcount(wn_pqueue queue);


#endif


