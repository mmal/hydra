#ifndef _H_GRIDS_COMMON_H_
#define _H_GRIDS_COMMON_H_

#include <stdlib.h>
#include <stdio.h>

#include "common/h_types.h"
#include "src/error_log/error_printf.h"

typedef struct
{
  H_DBL xL, xR;
  H_DBL h;
  int N, l, m;

  struct h_grid * offspring;
  struct h_grid * sibling;
  struct h_grid * parent;
  struct h_grid * neighbour;

  H_DBL t;

  H_DBL * x;
  H_DBL * u;
  
} h_grid;

typedef struct
{
  h_grid * g;
  void * params;
} h_union_grid;


h_grid * h_alloc_grid ( void );

void h_init_coarse_grid ( h_grid * g, H_DBL xL, H_DBL xR, int N );

void h_free_grid ( h_grid * g );

#endif /* _H_GRIDS_COMMON_H_ */
