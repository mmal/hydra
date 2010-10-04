

#include "update.h"


int _h_dbl_eq ( H_DBL a, H_DBL b )
{
  if ( fabs ( a-b ) <= DBL_EPSILON )
      return 1;
  else
      return 0;
}


int _h_update_grid ( h_grid *parent, h_grid *child, h_amrp *amrp  )
{
  VL((" *** Updating from child l=%d m=%d to parent l=%d m=%d\n",
      child->l, child->m, parent->l, parent->m ));

  int status;
  
  status = _h_update_grid_interior ( parent, child, amrp );
  if ( status != H_OK )
      return status;
  
  status = _h_update_grid_ghosts ( parent, child, amrp );

  return status;
}


int _h_update_grid_interior ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int i, j, r, Nchild, Nparent;

  int rratio = amrp->rr;
  int rank = 2;
  
  H_DBL *xparent = h_get_grid_positions ( parent );
  H_DBL *xchild = h_get_grid_positions ( child );

  H_DBL *uparent, *uchild;

  Nchild = child->N;
  Nparent = parent->N;
  
  for (i = 0; i < Nchild; i+=rratio) {
      for (j = 0; j < Nparent; j++) {
          if ( xchild[i] >= xparent[j] - DBL_EPSILON && xchild[i] <= xparent[j] + DBL_EPSILON ) {
              for (r = 0; r < rank; r++) {
                  uparent = h_get_grid_values ( parent, r );
                  uchild = h_get_grid_values ( child, r );
                  uparent[j] = uchild[i];
              }
              break;
          }
      }
  }
  return H_OK;
}



int _h_update_grid_ghosts ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int status;

  status = _h_update_grid_ghosts_left ( parent, child, amrp );
  if ( status != H_OK )
      return status;

  status = _h_update_grid_ghosts_right ( parent, child, amrp );
  if ( status != H_OK )
      return status;

  return H_OK;
}



int _h_update_grid_ghosts_right ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  return H_OK;
}



int _h_update_grid_ghosts_left ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int i, j, r, Nchild, Nparent;
  int Lghost, Rghost;

  int iparentL=-1, iparentR=-1, nparentL;
  
  int rratio = amrp->rr;
  int ngh = amrp->ngh;
  int rank = child->rank;
  
  
  H_DBL *xparent = h_get_grid_positions_wghosts ( parent );
  H_DBL *uparent = h_get_grid_values_wghosts ( parent, 0 );
  
  H_DBL *xchild = h_get_grid_positions_wghosts ( child );
  H_DBL *uchild = h_get_grid_values_wghosts ( child, 0 );

  H_DBL xLc = child->xL;
  H_DBL xRc = child->xR;

  H_DBL xLp = parent->xL;
  H_DBL xRp = parent->xR;

  H_DBL hparent = parent->h;

  H_DBL *x_to_interpolate;
  H_DBL *u_to_interpolate;

  Lghost= child->Lghost;
  Rghost= child->Rghost;
  Nchild = child->N;
  Nparent = parent->Ntotal;

  printf("Lghost=%d\n", Lghost);
  
  if ( Lghost > 0 ) {
      
      for (j = 0; j < Nparent; j++) {
          if ( _h_dbl_eq( xparent[j], xLc ) ) {
              iparentL = j;
              printf("%e %e\n", xparent[j], xLc );
              break;
          }
          /* if ( _h_dbl_eq( xparent[j], xchild[Lghost-1] ) ) */
          /*     iparentR = j; */
      }

      nparentL = (int) ( (xparent[iparentL] - xLp)/hparent ) + 1;
      
      if ( nparentL > ngh/rratio )
          nparentL = ngh/rratio;

      x_to_interpolate = (H_DBL*) malloc ( (nparentL+2)*sizeof( H_DBL ) ); 
      u_to_interpolate = (H_DBL*) malloc ( rank*(nparentL+2)*sizeof( H_DBL ) ); 

      for (i = 0; i < nparentL; i++) {
          /* printf( "i=%d xparent[%d]=%e\n", i, iparentL-nparentL+i+1, xparent[iparentL-nparentL+i+1] ); */
          x_to_interpolate[i] = xparent[iparentL-nparentL+i+1];

          uparent = h_get_grid_values_wghosts ( parent, 0 );

          x_to_interpolate[i] = xparent[iparentL-nparentL+i+1];
          u_to_interpolate[i] = uparent[iparentL-nparentL+i+1];
      }

      for (i = 0; i < 2; i++) {
          x_to_interpolate[nparentL+i] = xchild[Lghost+1+i];
          u_to_interpolate[nparentL+i] = uchild[Lghost+1+i];
      }
      
      FILE *fp = fopen("file", "w");
      for (i = 0; i < nparentL+2; i++) {
          fprintf( fp, "%e %e\n", x_to_interpolate[i], u_to_interpolate[i]);
      }
      fclose(fp);
      
      /* printf ( "iparentL=%d\n", iparentL); */
      /* printf ( "nparentL=%d\n", nparentL); */
      
      /* printf ( "%d %d\n", iparentL, iparentR ); */
      /* printf ( "xchild[i=%d] = %e\n", 0, xchild[0] ); */
      /* printf ( "xchild[i=%d] = %e\n", Lghost-1, xchild[Lghost-1] ); */
  }
  
  /* for (i = 0; i < Lghost; i++) { */
  /*     for (r = 0; r < rank; r++) { */
  /*         uchild = h_get_grid_values_wghosts ( child, r ); */
  /*         uchild[i] = 0.921; */
  /*     } */
  /* } */
  /* for (i = Nchild+Lghost; i < Nchild+Lghost+Rghost; i++) { */
  /*     for (r = 0; r < rank; r++) { */
  /*         uchild = h_get_grid_values_wghosts ( child, r ); */
  /*         uchild[i] = 0.921; */
  /*     } */
  /* } */

  /* gsl_interp_accel *acc */
  /*     = gsl_interp_accel_alloc (); */

  /* gsl_spline *spline */
  /*     = gsl_spline_alloc (gsl_interp_linear, nparentL+2); */
     
  /* gsl_spline_init (spline, x_to_interpolate, u_to_interpolate, nparentL+2); */

  /* for (i = 0; i < Lghost; i++) { */
  /*     uchild[i] = gsl_spline_eval (spline, xchild[i], acc); */
  /* } */
      
  /* gsl_spline_free (spline); */

  /* gsl_interp_accel_free (acc); */

}

int _h_update_glevel ( h_glevel *parent, h_glevel *child, h_amrp *amrp )
{
  VL((" *** Updating from child glevel l=%d to parent glevel l=%d\n",
      child->l, parent->l ));
  int status;
  int m, M;

  M = child->M;
  for (m = 0; m < M; m++) {
      status = _h_update_grid ( child->grid[m]->parent, child->grid[m], amrp );

      if ( status != H_OK )
              break;
  }
  /* należy znaleźć pozycję siatki gęstej na siatce rzadkiej */

  return status;
}
