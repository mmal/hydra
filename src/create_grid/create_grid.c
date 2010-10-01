

#include "create_grid.h"


int asd=0;



void _h_create_child_grid ( h_grid *parent, h_grid *child,
                            h_amrp *p, int m, int idL, int idR )
{
  /* ghost points, what with them ? */
  int i;

  int rr = p->rr; /* refinement ratio */

  int sp = p->ngh; /* ngh number of ghosts points */

  int N_c = (idR-idL)*rr+1; /* number of child grid points */
  
  int Lghost_c = 0, Rghost_c = 0; /* left and right ghost points of the child grid */

  H_DBL h_p = parent->h; /* spatial spacing of parent grid */
  
  H_DBL *x_c = h_get_grid_positions ( parent ); /* TODO: there was mistake ! */

  H_DBL xL_c = x_c[idL]; /* coordinate of the left end of the child grid */

  H_DBL xR_c = x_c[idR]; /* coordinate of the right end of the child grid */

  H_DBL xL_m, xR_m; /* coordinates of the ends of the master grid */
  
  /* h_grid *fine_grid = h_alloc_grid ( ); */

  /* h_grid *master_grid = h_alloc_grid ( ); */

  h_grid *master_grid;
  
  if ( parent->is_master == H_TRUE  ) { /* parent grid is a master grid */

      xL_m = parent->xL;
      xR_m = parent->xR;
      child->master = parent; /* this was */
      master_grid = (h_grid*) parent; /* missing */

  }
  else {
      /* parent grid is not a master grid */
      master_grid = (h_grid *) parent->master;
      xL_m = master_grid->xL;
      xR_m = master_grid->xR;
  }

 
  for (i = 1; i <= sp; i++) {
      if (xL_c - i*h_p/rr > xL_m)
          Lghost_c++;
      if (xR_c + i*h_p/rr < xR_m)
          Rghost_c++;
  }
  
  /* initializing fine grid */
  h_init_grid ( child, xL_c, xR_c, N_c, Lghost_c, Rghost_c,
                parent->rank, (parent->l)+1, m );

  child->master = (void *) master_grid;

  child->parent = (void *) parent;

  /* *child = fine_grid; */
}



int h_create_init_gset ( h_hms *hms )
{
  char *fnc_msg = "Create and initialize h_gset";

  int l, lmax, status;
  
  if ( hms == NULL )
      _STAT_MSG ( fnc_msg,
                  "h_hms is unallocated",
                  ERROR, 0 );

  l = 0;
  lmax = hms->amrp->lmax;
  
  while ( l < lmax  ) {

      status = h_create_init_glevel ( hms, l );
      VL(("l=%d\n",l));
      if (status != H_OK) {
          break;
      }
      
      l++;
  }
  
  return status;
}



int h_create_init_glevel ( h_hms *hms, int l )
{
  char *fnc_msg = "Create and initialize h_glevel";

  int m, M, status = H_ER;

  h_gset *gset;
  h_glevel *glevel;
  h_grid *grid;

  h_amrp *amrp;
  h_fnc *fnc;
  
  if ( hms == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_hms is unallocated",
                  ERROR, 0 );
      return status;
  }
  else if ( hms->gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  ERROR, 0 );
      return status;
  }
  else if ( hms->gset->glevel == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_glevel is unallocated",
                  ERROR, 0 );
      return status;
  }
  else {
      gset = hms->gset;
      amrp = hms->amrp;
      fnc = hms->fnc;
      
      /* check whether master grid is created and
       * properly initialized */
      if ( l == 0 ) {
          grid = h_point_to_master_grid ( gset );

          if ( grid == NULL ) {
              _STAT_MSG ( fnc_msg,
                          "master h_grid is unallocated",
                          ERROR, 0 );
              return status;
          }
          else if ( grid->x == NULL || grid->u == NULL ) {
              _STAT_MSG ( fnc_msg,
                          "master h_grid is uninitialized",
                          ERROR, 0 );
              return status;
          }
          else {
              _h_acd_to_grid ( grid, fnc );
              return H_OK;
          }
      }
      /* l > 0 */
      else {
          glevel = gset->glevel[l-1]; /* parent level (l-1) */

          M = glevel->M; /* # of grids in parent level (l-1) */

          /* allocating l-th level without grids */
          h_alloc_add_glevel ( gset, l, 0 );
          
          for (m = 0; m < M; m++) {
              
              status = h_create_init_grid ( glevel->grid[m], gset->glevel[l], amrp, fnc );
              
              if (status != H_OK) 
                  break;
          }
          
          return status;
      }
  }
}



int h_create_init_grid ( h_grid *pgrid, h_glevel *chglevel,
                         h_amrp *amrp, h_fnc *fnc )
{
  char *fnc_msg = "Create and initialize h_grid";

  int m, M, status = H_ER;
  
  int *id_fp, Nfp, *idL, *idR, Ngrids;

  
  if ( pgrid == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "parent h_grid is unallocated",
                  WARNING, 0 );
      return status;
  }
  else if ( chglevel == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "child h_glevel is unallocated",
                  WARNING, 0 );
      return status;
  }
  else if ( amrp == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_amrp is unallocated",
                  WARNING, 0 );
      return status;
  }
  else if ( fnc == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  WARNING, 0 );
      return status;
  }
  else {

      /* flag points in pgrid */
      h_flagging_points ( pgrid, amrp, fnc, &id_fp, &Nfp );  

      if ( Nfp > 0 ) {
          /* cluster flagged points in pgrid */
          h_clustering_flagged ( id_fp, Nfp, amrp->buf, pgrid->N, &idL, &idR, &Ngrids );

          /* save # of grids in chglevel */
          M = h_get_num_grids_in_glevel ( chglevel );
          
          /* add and alloc N grids in chglevel */
          h_alloc_add_N_grids_to_glevel ( chglevel, Ngrids );
          
          for (m = 0; m < Ngrids; m++) {
              /* alloc and initialize child grid in chglevel */
              _h_create_child_grid ( pgrid, chglevel->grid[M+m], amrp, m, idL[m], idR[m] );

              /* assign Cauchy data to child grid in chglevel */
              _h_acd_to_grid ( chglevel->grid[M+m], fnc );
          }

          /* free vectors needed for flagging */
          /* TODO: construct some flagging structure
           * containing these vectors and flagging
           * criterion function */
          
          if( id_fp!=NULL )
              free( id_fp );
          if( idL!=NULL )
              free( idL );
          if( idR!=NULL )
              free( idR );

          return H_OK;
      }
      else {
          _STAT_MSG ( fnc_msg,
                      "no flagged points at given glevel",
                      WARNING, 0 );
          return H_ER;
      }
  }
}



void _h_acd_to_grid ( h_grid *g, h_fnc *f )
{
  char *fnc_msg = "Assign Cauchy data to grid";

  int i, j, Ntotal;

  _fnc_1D fnc_ptr;

  if ( g == NULL || g->x == NULL || g->u == NULL )
      _STAT_MSG ( fnc_msg,
                  "h_grid is non initiated/unallocated",
                  ERROR, 0 );
  
  if ( g->rank != f->rank )
      _STAT_MSG ( fnc_msg,
                  "ranks of h_grid and h_fnc structures are not equal",
                  ERROR, 0 );
  
  Ntotal = g->Ntotal;
  
  /* loop through initial functions */
  for (i = 0; i < f->rank; i++) {
      
      fnc_ptr = f->C_da[i];
      
      /* loop through the grid points */
      for (j = 0; j < Ntotal; j++) {
          g->u[i*Ntotal+j] = fnc_ptr( g->x[j], f->params );
      }
  }
}



void _h_acd_to_glevel ( h_glevel *glevel, h_fnc *fnc )
{
  char *fnc_msg = "Assign Cauchy data to glevel";
  
  int m, M;

  if ( glevel == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_glevel is unallocated",
                  WARNING, 0 );
  }
  else if ( fnc == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  WARNING, 0 );
  }
  else {
      /* there is no need to check whether each grid
       * in glevel is allocated or not, this is
       * done in _h_acd_to_grid function
       */
      
      M = glevel->M; /* # of allocated grids in glevel */

      for (m = 0; m < M; m++) {
          _h_acd_to_grid ( glevel->grid[m], fnc );
      }
  }
}



/* jeżeli zestaw siatek został utworzony przez
 * używtkownika wcześniej */
void _h_acd_to_gset ( h_gset *gset, h_fnc *fnc )
{
  char *fnc_msg = "Assign Cauchy data to gset";
  
  int l, L;

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  WARNING, 0 );
  }
  else if ( fnc == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  WARNING, 0 );
  }
  else {
      /* there is no need to check whether each grid
       * in glevel is allocated or not, this is
       * done in _h_acd_to_glevel function
       */
      
      L = gset->L; /* # of allocated levels in gset */

      for (l = 0; l < L; l++) {
          _h_acd_to_glevel ( gset->glevel[l], fnc );
      }
  }
}
