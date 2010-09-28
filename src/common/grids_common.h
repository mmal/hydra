#ifndef _H_GRIDS_COMMON_H_
#define _H_GRIDS_COMMON_H_


#include <stdlib.h>
#include <stdio.h>


#include "src/common/types.h"
#include "src/stat_log/stat_msg.h"


typedef struct
{
  int N, l, m;
  int rank;                     /**< rank of the equation */
  
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
  
  H_DBL t, tlast;
  int Ncalls;                   /**< number of calls by the RHS equation */
  
  H_DBL *x;
  H_DBL *u;
  
  int Lghost, Rghost;
  int Ntotal;
  
} h_grid;


typedef struct
{
  int l;                        /**< level of grids */
  int M;                        /**< actual number of grids in glevel
                                 * struct */
  h_grid **grid;                /**< pointer to the table of grids
                                 * to the same level  */
} h_glevel;


typedef struct
{
  int L;                        /**< actual number of grids levels */
  
  h_glevel **glevel;            /**< pointer to the table of grids levels */
} h_gset;



h_grid *h_alloc_grid ( void );
void h_init_grid ( h_grid *g, H_DBL xL, H_DBL xR, int N, int Lghost, int Rghost, int rank, int l, int m );
void h_init_master_grid ( h_grid *g, H_DBL xL, H_DBL xR, int N, int rank );
H_DBL *h_get_grid_positions ( h_grid *g );
H_DBL *h_get_grid_positions_wghosts ( h_grid *g );
H_DBL *h_get_grid_values ( h_grid *g, int rank );
H_DBL *h_get_grid_values_wghosts ( h_grid *g, int rank );
void h_free_grid ( h_grid *g );
h_gset *h_alloc_gset ( void );
void h_free_gset ( h_gset *gset );
void h_alloc_add_glevel ( h_gset *gset, int l, int M );
void h_alloc_add_grid ( h_gset *gset, int l, int m );
void h_alloc_add_N_grids ( h_gset *gset, int l, int N );

void h_info_gset ( h_gset *gset );
void h_info_glevel ( h_glevel *glevel );
void h_info_grid ( h_grid *grid );
h_grid *h_point_to_grid ( h_gset *gset, int l, int m );
h_glevel *h_point_to_glevel ( h_gset *gset, int l );


#endif /* _H_GRIDS_COMMON_H_ */
