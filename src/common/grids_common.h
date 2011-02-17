#ifndef _H_GRIDS_COMMON_H_
#define _H_GRIDS_COMMON_H_


#include <stdlib.h>
#include <stdio.h>


#include "src/common/types.h"
#include "src/common/pos_common.h"
#include "src/common/amrp_common.h"
#include "src/stat_log/stat_msg.h"


typedef struct
{
  int N;                        /**< number of physical grid points */
  int rank;                     /**< rank of the equation */
  int l, m;                     /**< grid position in the gset */
  H_DBL xL, xR;                 /**< physical range of grid */
  H_DBL h, dt;                  /**< spatial and temporal step sizes */
  
  int is_master;                /**< marker if grid is a master grid */

  void *master;                 /**< pointer to the main grid */
  void *parent;                 /**< pointer to the parent grid */

  int Nchildren;                /**< number of children grids */
  
  H_DBL t, tlast;               /**< current time and the time of the grid in the previous step */
  int Ncalls;                   /**< number of calls by the RHS equation */
  
  H_DBL *x;                     /**< pointer to the grid positions */
  H_DBL *u;                     /**< pointer to the grid values */
  
  int Lghost, Rghost;           /**< number of left and right ghost points */
  int Ntotal;                   /**< total number of grids points, i.e. Ntotal=N+Lghost+Rghost  */

  h_pos *pos_on_parent;         /**< position on parent grid */
  h_pos *pos_on_parent_wgh;     /**< position on parent grid with ghost points */
  
  int id_xL, id_xR;             /**< ??? */
  int id_Lgh, id_Rgh;           /**< ??? */

  H_DBL xL_gh, xR_gh;           /**< total range of grid */
  
} h_grid;


typedef struct
{
  int l;                        /**< level of grids */
  int M;                        /**< actual number of grids in glevel struct */
  h_grid **grid;                /**< pointer to the table of grids to the same level  */
} h_glevel;


typedef struct
{
  int L;                        /**< actual number of grids levels */  
  h_glevel **glevel;            /**< pointer to the table of grids levels */
} h_gset;



h_grid *h_alloc_grid ( void );
void h_init_grid ( h_grid *g, H_DBL xL, H_DBL xR, int N, int Lghost, int Rghost, int rank, int l, int m, h_amrp *p );
void h_init_master_grid ( h_grid *g, H_DBL xL, H_DBL xR, int N, int rank, h_amrp *p );
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

/* void h_alloc_add_N_grids_to_gset ( h_gset *gset, int l, int N ); */

void h_alloc_add_N_grids_to_glevel ( h_glevel *glevel, int N );
void h_info_gset ( h_gset *gset );
void h_info_glevel ( h_glevel *glevel );
void h_info_grid ( h_grid *grid );
h_grid *h_point_to_grid ( h_gset *gset, int l, int m );
h_grid *h_point_to_master_grid ( h_gset *gset );
h_glevel *h_point_to_glevel ( h_gset *gset, int l );
int h_get_num_glevels_in_gset ( h_gset *gset );
int h_get_num_grids_in_gset ( h_gset *gset, int l );
int h_get_num_grids_in_glevel ( h_glevel *glevel );

void h_free_rem_glevel ( h_gset *gset, int l );

#endif /* _H_GRIDS_COMMON_H_ */
