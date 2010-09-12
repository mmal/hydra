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



void h_flagging_points ( h_grid *g, h_amrp *p,
                         int (*flagging_criterion)( h_grid *, h_amrp *, H_DBL * ),
                         int **id_fp, int *Nfp )
{
  int i, N, stat;

  int *id_fp_t = NULL;
  
  H_DBL err_tol;

  H_DBL *tau;


  if ( g==NULL || p==NULL || flagging_criterion==NULL ) {
      _STAT_MSG("Flagging points","empty parameters",ERROR,0);
  }

  N = g->N;
  err_tol = p->errt;

  printf("TEST g->N=%d\n", g->N);
  tau = (H_DBL*) malloc( N*sizeof( H_DBL ) );

  if ( tau==NULL ) {
      _STAT_MSG("Flagging points","cannot allocate memory for tau",ERROR,0);
  }
  
  stat = flagging_criterion ( g, p, tau );

  if ( stat!=H_OK ) {
      _STAT_MSG("Flagging points","flagging criterion failed",ERROR,0);
  }
  
  *Nfp = 0; /* at the beginning there are no flagged points */

  if( id_fp_t == NULL ) printf( "id_fp_t == NULL\n" );
  
  id_fp_t = (int*) realloc( id_fp_t, 10*sizeof(int) );

  /* if( id_fp_t == NULL ) printf( "reallocating \n id_fp_t == NULL\n" ); */

  
  for (i = 0; i < N; i++) {
      if ( tau[i] > err_tol ) {
          /* flag i-th point  */
          ++*Nfp;
          printf(" Nfp=%d\n", *Nfp);
          id_fp_t = (int *) realloc( id_fp_t, (*Nfp)*sizeof(int) );
          id_fp_t[(*Nfp)-1] = i;
      }
  }

  for (i = 0; i < *Nfp; i++) {
      printf("i=%d, id_fp=%d\n", i, id_fp_t[i]);
  }

  *id_fp = id_fp_t;

  printf("flagging.c TEST\n");

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
      printf(" TEST i=%d\n", i);
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
