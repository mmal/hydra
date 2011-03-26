
#include "regrid.h"



int h_regrid ( h_hms * hms )
{
  char *fnc_msg = "re-griding algorithm";

  h_gset *gset = hms->gset;
  h_amrp *amrp = hms->amrp;
  h_fnc *fnc = hms->fnc;

  int l, L;
  int status;
  
  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  WARNING, 0 );
      return H_ER;
  }
  else if ( amrp == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_amrp is unallocated",
                  WARNING, 0 );
      return H_ER;
  }
  else if ( fnc == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  WARNING, 0 );
      return H_ER;
  }
  else {
      L = gset->L;
      /* printf("in  L=%d\n", L); */
      /* sleep(1); */
      
      /* if ( L < LMAX )
       *  create new level if needed for l=L...LMAX
       *  then regrid lover levels */
      
      for (l = L-1; l > 0; l--) {
          status = _h_regrid_glevel ( gset, amrp, fnc, l );
      }
      /* printf("out L=%d\n", gset->L); */
      /* sleep(1); */
      /* if ( L == 2 ) */
      /*     status = _h_regrid_glevel ( gset, amrp, fnc, 1 ); */
      /* status = _h_regrid_glevel ( gset, amrp, fnc, 1 ); */

      return status;
  }
}



/* l - ktory level 'przerabiamy', czyli korzystamy
 * z levelu (l-1) */
int _h_regrid_glevel ( h_gset *gset, h_amrp *amrp, h_fnc *fnc, int l )
{
  VL((" *** Re-griding level l=%d\n",l ));
  char *fnc_msg = "Re-grid h_glevel";

  int m, M, j, J;
  
  int *id_fp, Nfp, *idL, *idR, Ngrids;

  h_grid *parent_grid;
  /* h_grid *child_grid; */

  h_glevel *parent_glevel = gset->glevel[l-1];
  h_glevel *child_glevel;
  
  /* allocating l-th level without grids */
  /* h_alloc_add_glevel ( gset, l, 0 ); */
  /* niszcze poprzedni istniejacy level l */
  
  /* h_free_rem_glevel ( gset, l ); */ /* TODO: z ta linijka program sie
                                        * wysypywal ???????? */
  h_alloc_add_glevel ( gset, l, 0 );
  /* gset->L = gset->L-1; */
  child_glevel = gset->glevel[l];

  M=parent_glevel->M; /* brakowalo tej linii */
  
  for (m = 0; m < M; m++) {
      /* regrid each grid from parent grid  */
      parent_grid = parent_glevel->grid[m];
      
      /* flag points in parent */
      h_flagging_points ( parent_grid, amrp, fnc, &id_fp, &Nfp );  

      printf("Nfp=%d\n", Nfp);
      /* sleep(1); */
      if ( Nfp > 0 ) {
          /* cluster flagged points in parent */
          h_clustering_flagged ( id_fp, Nfp, amrp->buf, parent_grid->N, &idL, &idR, &Ngrids );

          /* save # of grids in child_glevel */
          J = h_get_num_grids_in_glevel ( child_glevel );
          printf("J=%d\n", J);
          J = 0;/* printf("M=%d\n", M); */
          /* sleep(1); */
          /* add and alloc N grids in child_glevel */
          h_alloc_add_N_grids_to_glevel ( child_glevel, Ngrids );
          
          for (j = 0; j < Ngrids; j++) {
              /* alloc and initialize child grid in child_glevel */
              _h_create_child_grid ( parent_grid, child_glevel->grid[J+j], amrp, j, idL[j], idR[j] );

              child_glevel->grid[J+j]->parent = parent_grid;
              /* child_glevel->grid[J+j]->master = parent_grid->master; */


              /* synchronize time in grids */
              child_glevel->grid[J+j]->t = parent_grid->t;
              
              /* interpolate from parent_grid to child_grid */
              _h_interpolate_from_parent_to_child_grid ( parent_grid, child_glevel->grid[J+j] );


          }
          child_glevel->M=J+Ngrids;
          
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

      }
      else {
          _STAT_MSG ( fnc_msg,
                      "no flagged points at given glevel",
                      WARNING, 0 );
          return H_ER;
      }
  }
  gset->glevel[l] = child_glevel; /* \bug ? */

  if (child_glevel->M==0)
    {
        gset->L = gset->L-1;
    }
  return H_OK;
}
