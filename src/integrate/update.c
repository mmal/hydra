

#include "update.h"

/* gsl_interp_linear */
/* gsl_interp_polynomial */
/* gsl_interp_cspline */


#define INTERP_TYPE gsl_interp_polynomial
#define N_CHILD 2


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
  int i, j, r, Nchild, Nparent;
  int Lghost, Rghost;

  int iparentR=-1, NparentR, NchildR=N_CHILD;
  
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

  H_DBL t = child->t;

  Lghost= child->Lghost;
  Rghost= child->Rghost;
  Nchild = child->N;
  Nparent = parent->Ntotal;

  if ( Rghost > 0 ) {
      for (j = 0; j < Nparent; j++) {
          if ( _h_dbl_eq( xparent[j], xRc ) ) {
              iparentR = j;
              printf("iparentR = %d \n", iparentR );
              break;
          }
      }

      NparentR = (int) ( ( xRp - xparent[iparentR] )/hparent ) + 1;
      printf("NparentR = %d \n", NparentR );
      if ( NparentR > ngh/rratio )
          NparentR = ngh/rratio;

      x_to_interpolate = (H_DBL*) malloc ( (NparentR+NchildR)*sizeof( H_DBL ) ); 
      u_to_interpolate = (H_DBL*) malloc ( rank*(NparentR+NchildR)*sizeof( H_DBL ) ); 
      
      for (i = 0; i < NparentR; i++) {
          printf( "i=%d xparent[%d]=%e\n", i, iparentR+i, xparent[iparentR+i] );
          x_to_interpolate[i+NchildR] = xparent[iparentR+i];

          for (r = 0; r < rank; r++) {
              uparent = h_get_grid_values_wghosts ( parent, r );
              u_to_interpolate[r*(NparentR+NchildR) + i +NchildR] = uparent[iparentR+i];
          }
      }

      /* adding child points for interpolation */
      for (i = 0; i < NchildR; i++) {
          x_to_interpolate[i] = xchild[Lghost-(NchildR-i-6)];
          
          for (r = 0; r < rank; r++) {
              uchild = h_get_grid_values_wghosts ( child, r );
              
              u_to_interpolate[r*(NparentR+NchildR) +  i] = uchild[Lghost-(NchildR-i-6)];
          }
      }
      
      
      
      FILE *fp = fopen("file", "w");
      for (i = 0; i < NparentR+NchildR; i++) {
          fprintf( fp, "%e %e\n", x_to_interpolate[i], u_to_interpolate[i]);
      }
      fclose(fp);

      
      for (r = 0; r < rank; r++)
        {
            uchild = h_get_grid_values_wghosts ( child, r );
            u_to_interpolate += r*(NparentR+NchildR);
            
            gsl_interp_accel *acc
                = gsl_interp_accel_alloc ();
            
            gsl_spline *spline
                = gsl_spline_alloc (INTERP_TYPE, NparentR+NchildR);
            
            gsl_spline_init (spline, x_to_interpolate, u_to_interpolate, NparentR+NchildR);
            
          for (i = 0; i < Rghost; i++) {
              if ( r == 0 )
                  uchild[Lghost+Nchild+i] =
                      sin(2*M_PI*xchild[Lghost+Nchild+i])*cos(2*M_PI*t);
              if ( r == 1 )
                  uchild[Lghost+Nchild+i] =
                      -2*M_PI*sin(2*M_PI*xchild[Lghost+Nchild+i])*sin(2*M_PI*t);
                  
              /* uchild[Lghost+Nchild+i] = */
              /*     gsl_spline_eval (spline, xchild[Lghost+Nchild+i], acc); */
          }
          
          gsl_spline_free (spline);
          
          gsl_interp_accel_free (acc);
      }

      
  }
  return H_OK;
}



int _h_update_grid_ghosts_left ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int i, j, r, Nchild, Nparent;
  int Lghost, Rghost;

  int iparentL=-1, iparentR=-1, NparentL, NchildL=N_CHILD;
  
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

  H_DBL t = child->t;

  Lghost= child->Lghost;
  Rghost= child->Rghost;
  Nchild = child->N;
  Nparent = parent->Ntotal;

  if ( Lghost > 0 ) {
      for (j = 0; j < Nparent; j++) {
          if ( _h_dbl_eq( xparent[j], xLc ) ) {
              iparentL = j;
              /* printf("%e %e\n", xparent[j], xLc ); */
              break;
          }
      }

      NparentL = (int) ( (xparent[iparentL] - xLp)/hparent ) + 1;
      
      if ( NparentL > ngh/rratio )
          NparentL = ngh/rratio;

      x_to_interpolate = (H_DBL*) malloc ( (NparentL+NchildL)*sizeof( H_DBL ) ); 
      u_to_interpolate = (H_DBL*) malloc ( rank*(NparentL+NchildL)*sizeof( H_DBL ) ); 
      
      for (i = 0; i < NparentL; i++) {
          /* printf( "i=%d xparent[%d]=%e\n", i, iparentL-nparentL+i+1, xparent[iparentL-nparentL+i+1] ); */
          x_to_interpolate[i] = xparent[iparentL-NparentL+i+1];


          for (r = 0; r < rank; r++) {
              uparent = h_get_grid_values_wghosts ( parent, r );
              u_to_interpolate[r*(NparentL+NchildL) + i] = uparent[iparentL-NparentL+i+1];
          }
      }

      /* adding child points for interpolation */
      for (i = 0; i < NchildL; i++) {
          x_to_interpolate[NparentL+i] = xchild[Lghost+1+i];

          for (r = 0; r < rank; r++) {
              uchild = h_get_grid_values_wghosts ( child, r );

              u_to_interpolate[r*(NparentL+NchildL) + NparentL + i] = uchild[Lghost+1+i];
          }
      }
      
      
      
      FILE *fp = fopen("file", "w");
      for (i = 0; i < NparentL+NchildL; i++) {
          fprintf( fp, "%e %e\n", x_to_interpolate[i], u_to_interpolate[i]);
      }
      fclose(fp);

      
      for (r = 0; r < rank; r++)
        {
            uchild = h_get_grid_values_wghosts ( child, r );
            u_to_interpolate += r*(NparentL+NchildL);
            
            gsl_interp_accel *acc
                = gsl_interp_accel_alloc ();
            
            gsl_spline *spline
                = gsl_spline_alloc (INTERP_TYPE, NparentL+NchildL);
            
            gsl_spline_init (spline, x_to_interpolate, u_to_interpolate, NparentL+NchildL);
            
          for (i = 0; i < Lghost; i++) {
              if ( r == 0 )
                  uchild[i] =
                      sin(2*M_PI*xchild[i])*cos(2*M_PI*t);
              if ( r == 1 )
                  uchild[i] =
                      -2*M_PI*sin(2*M_PI*xchild[i])*sin(2*M_PI*t);

              /* uchild[i] = gsl_spline_eval (spline, xchild[i], acc); */
          }
          
          gsl_spline_free (spline);
          
          gsl_interp_accel_free (acc);
      }

      
  }
  return H_OK;
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
