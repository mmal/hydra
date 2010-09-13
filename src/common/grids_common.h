#ifndef _H_GRIDS_COMMON_H_
#define _H_GRIDS_COMMON_H_


#include <stdlib.h>
#include <stdio.h>


#include "src/common/types.h"
#include "src/stat_log/stat_msg.h"


typedef struct
{
  int N, l, m;
  int rank;
  
  H_DBL xL, xR;
  H_DBL h, dt;
  
  int is_master;
  void *master;
  
  void *offspring;
  void *sibling;
  void *parent;
  void *neighbour;

  void *Lsibling;
  void *Rsibling;
  
  void **children;
  int Nchildren;
  
  H_DBL t;
  
  H_DBL *x;
  H_DBL *u;

  int Lghost, Rghost;
  
} h_grid;



h_grid *h_alloc_grid ( void );

void h_init_grid ( h_grid *g, H_DBL xL, H_DBL xR,
                   int N, int Lghost, int Rghost,
                   int rank, int l, int m );

void h_init_coarse_grid ( h_grid *g, H_DBL xL, H_DBL xR,
                          int N, int rank );

void h_free_grid ( h_grid * g );

#endif /* _H_GRIDS_COMMON_H_ */
