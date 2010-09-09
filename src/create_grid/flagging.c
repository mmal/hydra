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



void h_flagging_points ( h_grid * g, h_amrp * p,
                         int (*flagging_criterion)( h_grid *, h_amrp *, H_DBL * ),
                         int * id_fp , int * Nfp )
{
  int i, stat;

  size_t H_DBL_size = sizeof( H_DBL );

  H_DBL err_tolerance;

  H_DBL *tau;


  if ( g==NULL || p==NULL || flagging_criterion==NULL ) {
      _STAT_MSG("Flagging points","empty parameters",ERROR,0);
  }

  err_tolerance = p->errt;

  tau = (H_DBL*) malloc( g->N*H_DBL_size );

  if ( tau==NULL ) {
      _STAT_MSG("Flagging points","cannot allocate memory for tau",ERROR,0);
  }
  
  stat = flagging_criterion ( g, p, tau );

  if ( stat!=H_OK ) {
      _STAT_MSG("Flagging points","flagging criterion failed",ERROR,0);
  }
  
  *Nfp = 0; /* at the beginning there are no flagged points */
  
  for (i = 0; i < g->N; i++) {
      if ( tau[i] > err_tolerance ) {
          /* flag i-th point  */
          *Nfp++;
          id_fp = (int *) realloc( id_fp, *Nfp*H_DBL_size );
          id_fp[*Nfp-1] = i;
      }
  }

  free( tau );  
}



void h_clustering_flagged ( int * id_fp , int Nfp, int buf, int Ncoarse,
                            int * idR, int * idL, int *Ngrids )
{
  int i, in_grid = 0;

  size_t H_DBL_size = sizeof( H_DBL );
  
  for (i = 0; i < Nfp; i++) {
      if (in_grid==0 && (id_fp[i]-buf)<0) {
          idL = (int*) realloc(idL, H_DBL_size );

          idL[0] = 0;
          in_grid = 1;
      }
      else if (in_grid==0 && (id_fp[i]-buf)>0) {
          idL = (int*) realloc(idL, H_DBL_size );

          idL[0] = id_fp[i] - buf;
          in_grid = 1;
      }
      else if (in_grid==1 && (id_fp[i]-2*buf-id_fp[i-1])>=1) {
          idR = (int*) realloc(idR, (*Ngrids+1)*H_DBL_size );

          idR[*Ngrids] = id_fp[i-1]+buf;

          idL = (int*) realloc(idR, (*Ngrids+2)*H_DBL_size );
          idL[*Ngrids+1] = id_fp[i]-buf;
          
          Ngrids++;
      }
      else if (in_grid==1 && (id_fp[i]+buf)>=Ncoarse-1) {
          idR = (int*) realloc(idR, (*Ngrids+1)*H_DBL_size );

          idR[*Ngrids]=Ncoarse-1;

          Ngrids++;
          
          i=Nfp;
      }
      else if (in_grid==1 && (id_fp[i]+buf)<Ncoarse-1 && i==Nfp-1) {
          idR = (int*) realloc(idR, (*Ngrids+1)*H_DBL_size );
          
          idR[*Ngrids]=id_fp[i]+buf;

          Ngrids++;
      }
  }
}
