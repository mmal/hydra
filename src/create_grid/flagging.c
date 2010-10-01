/**
 * @file   flagging.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Tue Sep  7 16:24:24 2010
 * 
 * @brief  This file is a set of functions needed for flag and
 * collect coarse grid points needed refinement.
 * 
 * 
 */


#include "flagging.h"



void h_flagging_points ( h_grid *grid, h_amrp *amrp, h_fnc *fnc,
                         int **id_fp, int *Nfp )
{
  char *fnc_msg = "Flagging points";
    
  int i, N, status;
  int *id_fp_t = NULL;
  
  H_DBL err_tol;
  H_DBL *tau;

  _flag_crit *_fc;

  N = grid->N;

  err_tol = amrp->errt;

  tau = (H_DBL*) malloc( N*sizeof( H_DBL ) );

  if ( tau==NULL ) {
      _STAT_MSG( fnc_msg,
                 "cannot allocate memory for tau",
                 ERROR, 0);
  }
  
  _fc = &fnc->fc; /* TODO */
  
  status = (*_fc)( grid, amrp, fnc, tau );

  if ( status!=H_OK ) {
      _STAT_MSG( fnc_msg,
                 "flagging criterion failed",
                 ERROR, 0);
  }
  
  /* at the beginning there are no flagged points */
  *Nfp = 0; 

  id_fp_t = (int*) realloc( id_fp_t, 10*sizeof(int) );

 
  /* checking whether the returned value exceeds
   * the error tolerance, loop over the real grid points */
  for (i = 0; i < N; i++) {

      if ( tau[i] > err_tol ) {
          ++*Nfp; /* flag i-th point and realloc flagged points
                   * matrix id */
          id_fp_t = (int *) realloc( id_fp_t, (*Nfp)*sizeof(int) );
          id_fp_t[(*Nfp)-1] = i; /* save flagged point id */
      }
  }

  *id_fp = id_fp_t;

  if ( tau!= NULL )
      free( tau );
}


void h_clustering_flagged ( int *id_fp , int Nfp, int buf, int Ncoarse,
                            int **idL, int **idR, int *Ngrids )
{
  int i, in_grid = 0;

  int *idL_t = NULL;
  int *idR_t = NULL;
  
  size_t int_size = sizeof( int );

  *Ngrids = 0;
  
  for (i = 0; i < Nfp; i++) {
      if (in_grid==0 && (id_fp[i]-buf)<=0) {
          idL_t = (int*) realloc( idL_t, int_size );
          
          idL_t[0] = 0;
          in_grid = 1;
      }
      else if (in_grid==0 && (id_fp[i]-buf)>0) {
          idL_t = (int*) realloc( idL_t, int_size );

          idL_t[0] = id_fp[i] - buf;
          in_grid = 1;
      }
      else if (in_grid==1 && (id_fp[i]-2*buf-id_fp[i-1])>0) {
          idR_t = (int*) realloc( idR_t, (*Ngrids+1)*int_size );

          idR_t[*Ngrids] = id_fp[i-1]+buf;

          idL_t = (int*) realloc( idL_t, (*Ngrids+2)*int_size );
          idL_t[*Ngrids+1] = id_fp[i]-buf;
          
          (*Ngrids)++;
      }
      else if (in_grid==1 && (id_fp[i]+buf)>=Ncoarse-1) {
          idR_t = (int*) realloc( idR_t, (*Ngrids+1)*int_size );

          idR_t[*Ngrids]=Ncoarse-1;

          (*Ngrids)++;
          
          i=Nfp;
      }
      else if (in_grid==1 && (id_fp[i]+buf)<Ncoarse-1 && i==Nfp-1) {
          idR_t = (int*) realloc( idR_t, (*Ngrids+1)*int_size );
          
          idR_t[*Ngrids]=id_fp[i]+buf;

          (*Ngrids)++;
      }
  }


  *idL = idL_t;
  *idR = idR_t;

}
