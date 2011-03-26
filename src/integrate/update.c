

#include "update.h"

/* gsl_interp_linear */
/* gsl_interp_polynomial */
/* gsl_interp_cspline */

#define INTERP_TYPE gsl_interp_polynomial
#define N_CHILD 2
#define NPOINTS 6

int _h_dbl_eq ( H_DBL a, H_DBL b )
{
  if ( fabs ( a-b ) <= DBL_EPSILON )
      return 1;
  else
      return 0;
}


H_DBL *_h_find_5_nearest ( H_DBL x, h_grid *grid, h_amrp *amrp  )
{
  char *fnc_msg = "Find five nearest points to x";

  H_DBL xL_gh = grid->xL_gh; 
  H_DBL xR_gh = grid->xR_gh; 

  H_DBL *xnear;

  H_DBL *xgrid = h_get_grid_positions_wghosts ( grid );

  int i, j, N = grid->Ntotal;
  
  const int Npts = NPOINTS;
  
  xnear = (H_DBL*) malloc ( Npts*sizeof( H_DBL ) );

  if ( xnear != NULL ) {

      if ( x >= xgrid[N-1] ) {
          for (j = 0; j < Npts; j++) {
              xnear[j] = xgrid[N-1-(Npts-1)+j];
          }
      }
      else if ( x <= xgrid[0] ) {
          for (j = 0; j < Npts; j++) {
              xnear[j] = xgrid[j];
          }
      }
      else {
          /* iterating all points in the grid */
          for (i = 0; i < N; i++) {
              if ( ( xgrid[i]-x )>= 0. ){
                  if ( i>1 && i < N-2 ) {
                      for (j = 0; j < Npts; j++) {
                          xnear[j] = xgrid[i-Npts/2+j];
                      }
                  }
                  /* else if ( i==0 ) { */
                  /*     for (j = 0; j < 5; j++) { */
                  /*         xnear[j] = xgrid[i+j]; */
                  /*     } */
                  /* } */
                  else if ( i==1 ) {
                      for (j = 0; j < Npts; j++) {
                          xnear[j] = xgrid[i-1+j];
                      }
                  }
                  else if ( i==N-2 ) {
                      for (j = 0; j < Npts; j++) {
                          xnear[j] = xgrid[i-(Npts-2)+j];
                      }
                  }
                  /* else if ( i==N-1 ) { */
                  /*     for (j = 0; j < 5; j++) { */
                  /*         xnear[j] = xgrid[i-4+j]; */
                  /*     } */
                  /* } */
              
                  break;
              }
          }
      }
  }
      
  
  return xnear;
}

int *_h_find_5_nearest_indices ( H_DBL x, h_grid *grid, h_amrp *amrp  )
{
  char *fnc_msg = "Find indices of five nearest points to x";

  H_DBL xL_gh = grid->xL_gh;
  H_DBL xR_gh = grid->xR_gh; 

  int *inear;

  H_DBL *xgrid = h_get_grid_positions_wghosts ( grid );

  int i, j, N = grid->Ntotal;
  
  const int Npts = NPOINTS;
  
  inear = (int*) malloc ( Npts*sizeof( int ) );

  if ( inear != NULL ) {



      if ( x >= xgrid[N-1] ) {
          for (j = 0; j < Npts; j++) {
              inear[j] = N-1-(Npts-1)+j;
          }
      }
      else if ( x <= xgrid[0] ) {
          for (j = 0; j < Npts; j++) {
              inear[j] = j;
          }
      }
      else {
          /* iterating all points in the grid */
          for (i = 0; i < N; i++) {
              if ( ( xgrid[i]-x )>= 0. ){
                  if ( i>1 && i < N-2 ) {
                      for (j = 0; j < Npts; j++) {
                          inear[j] = i-Npts/2+j;
                      }
                  }
                  else if ( i==1 ) {
                      for (j = 0; j < Npts; j++) {
                          inear[j] = i-1+j;
                      }
                  }
                  else if ( i==N-2 ) {
                      for (j = 0; j < Npts; j++) {
                          inear[j] = i-(Npts-2)+j;
                      }
                  }
                  break;
              }
          }
      }
  }
      
  
  return inear;
}



int _h_update_grid ( h_grid *parent, h_grid *child, h_amrp *amrp  )
{
  VL((" *** Updating from child l=%d m=%d to parent l=%d m=%d\n",
      child->l, child->m, parent->l, parent->m ));

  char *fnc_msg = "Updating from child to parent grid";

  int status;

  printf(" **** l_child=%g, l_parent=%g\n", child->t, parent->t );

  if ( _h_dbl_eq ( parent->t, child->t ) != 1 ) {
      _STAT_MSG ( fnc_msg,
                  "time of the parent and the child grid not equal",
                  ERROR, 0 );
      return H_ER;
  }
  else {
      status = _h_update_grid_interior ( parent, child, amrp );
      if ( status != H_OK )
          return status;
  
      status = _h_update_grid_ghosts_new_new ( parent, child, amrp );
      
      /* status = _h_update_grid_all ( parent, child, amrp ); */
  }
  
  return status;
}



int _h_update_grid_all  ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int i, r, rank = 2;
  H_DBL *xchild = h_get_grid_positions_wghosts ( child );
  H_DBL *uchild;

  H_DBL t = child->t;
  
  int Lghost, Rghost, Nchild;

  H_DBL tmp;
  tmp = parent->t;
  tmp = amrp->errt;
  
  Lghost = child->Lghost;
  Rghost = child->Rghost;  
  Nchild = child->N;

  for (r = 0; r < rank; r++) {

      uchild = h_get_grid_values_wghosts ( child, r );
      
      for (i = 0; i < child->Ntotal; i++) {
          if ( r == 0 )
              uchild[i] =
                  sin(2*M_PI*xchild[i])*cos(2*M_PI*t);
          if ( r == 1 )
              uchild[i] =
                  -2*M_PI*sin(2*M_PI*xchild[i])*sin(2*M_PI*t);
      }

      /* for (i = 0; i < Rghost; i++) { */
      /*     if ( r == 0 ) */
      /*         uchild[Lghost+Nchild+i] = */
      /*             sin(2*M_PI*xchild[Lghost+Nchild+i])*cos(2*M_PI*t); */
      /*     if ( r == 1 ) */
      /*         uchild[Lghost+Nchild+i] = */
      /*             -2*M_PI*sin(2*M_PI*xchild[Lghost+Nchild+i])*sin(2*M_PI*t); */
      /* } */
  }

  return H_OK;
}




int _h_update_grid_interior ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int i, j, r, Nchild, Nparent;

  int rratio = amrp->rr;
  int rank = parent->rank;
  
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



int _h_update_grid_ghosts_new ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int status;

  int i, j, r;
  int *inear; /* indices of nearest points */

  int rank = child->rank;
  int Lghost= child->Lghost;
  int Rghost= child->Rghost;
  int Nchild = child->N;

  H_DBL *xchild = h_get_grid_positions_wghosts ( child );
  H_DBL *xparent = h_get_grid_positions_wghosts ( parent );

  H_DBL *uchild, *uparent;
  
  H_DBL *x_to_interpolate;
  H_DBL *u_to_interpolate;

  x_to_interpolate = (H_DBL*) malloc ( NPOINTS*sizeof( H_DBL ) ); 
  u_to_interpolate = (H_DBL*) malloc ( NPOINTS*sizeof( H_DBL ) ); 

  
  /* update left ghost points */
  /* wskaznik i numeruje punkty siatki child ktorych wartosci mamy zinterpolowac */
  for (i = 0; i < Lghost; i++) {

      /* znaleziono indeksy punktów z któych będziemy interpolować */ 
      inear = _h_find_5_nearest_indices ( xchild[i], parent, amrp );

      for (j = 0; j < NPOINTS; j++) {
          x_to_interpolate[j] = xparent[inear[j]];
      }

      /* interpolujemy r-ty rząd */
      for (r = 0; r < rank; r++)
        {
            uchild = h_get_grid_values_wghosts ( child, r );
            uparent = h_get_grid_values_wghosts ( parent, r );

            for (j = 0; j < NPOINTS; j++) {
                u_to_interpolate[j] = uparent[inear[j]];
            }

            gsl_interp_accel *acc
                = gsl_interp_accel_alloc ();
            
            gsl_spline *spline
                = gsl_spline_alloc (INTERP_TYPE, NPOINTS);
            
            gsl_spline_init (spline, x_to_interpolate, u_to_interpolate, NPOINTS);
            
                  
            uchild[i] =
                gsl_spline_eval (spline, xchild[i], acc);
          
          gsl_spline_free (spline);
          
          gsl_interp_accel_free (acc);
        }
      if ( inear != NULL ) {
          free ( inear );
      }
  }


  /* update right ghost points */
  /* wskaznik i numeruje punkty siatki child ktorych wartosci mamy zinterpolowac */
  for (i = Nchild+Lghost; i < Nchild+Lghost+Rghost; i++) {

      /* znaleziono indeksy punktów z któych będziemy interpolować */ 
      inear = _h_find_5_nearest_indices ( xchild[i], parent, amrp );

      for (j = 0; j < NPOINTS; j++) {
          x_to_interpolate[j] = xparent[inear[j]];
      }

      /* interpolujemy r-ty rząd */
      for (r = 0; r < rank; r++)
        {
            uchild = h_get_grid_values_wghosts ( child, r );
            uparent = h_get_grid_values_wghosts ( parent, r );

            for (j = 0; j < NPOINTS; j++) {
                u_to_interpolate[j] = uparent[inear[j]];
            }

            gsl_interp_accel *acc
                = gsl_interp_accel_alloc ();
            
            gsl_spline *spline
                = gsl_spline_alloc (INTERP_TYPE, NPOINTS);
            
            gsl_spline_init (spline, x_to_interpolate, u_to_interpolate, NPOINTS);
            

            uchild[i] =
                gsl_spline_eval (spline, xchild[i], acc);

            /* child->u[i+r*(Nchild+Lghost+Rghost)] = */
            /*     gsl_spline_eval (spline, xchild[i], acc); */

            
          gsl_spline_free (spline);
          
          gsl_interp_accel_free (acc);
        }
      if ( inear != NULL ) {
          free ( inear );
      }
  }
}





int _h_update_grid_ghosts_new_new ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int status;

  int i, j, r;
  int *inear; /* indices of nearest points */

  int rank = child->rank;
  int Lghost= child->Lghost;
  int Rghost= child->Rghost;
  int Nchild = child->N;
  int Nparent = parent->Ntotal;

  H_DBL *xchild = h_get_grid_positions_wghosts ( child );
  H_DBL *xparent = h_get_grid_positions_wghosts ( parent );

  H_DBL *uchild, *uparent, *u2parent;
  
  /* interpolujemy r-ty rzad */
  for (r = 0; r < rank; r++)
    {
        uchild = h_get_grid_values_wghosts ( child, r );
        uparent = h_get_grid_values_wghosts ( parent, r );

        /* licze 2 pochodna przestrzenna funkcji dla
         * siatki parent */ 
        u2parent = _h_compute_D2 (xparent, uparent, Nparent);

        /* interpoluje lewe punkty siatki child */
        for (i = 0; i < Lghost; i++) {

            _h_splint (xparent, uparent,
                       u2parent,
                       Nparent,
                       xchild[i],
                       &uchild[i]);
        }
        
        /* interpoluje prawe punkty siatki child */
        for (i = Nchild+Lghost; i < Nchild+Lghost+Rghost; i++) {
            _h_splint (xparent, uparent,
                       u2parent,
                       Nparent,
                       xchild[i],
                       &uchild[i]);
        }

        /* zwalniam pamiec zaalokowana dla drugich pochodnych
         * przestrzennych u2parent */
        if ( u2parent != NULL ) {
            free ( u2parent );
        }
    }
}

















int _h_update_grid_ghosts_new2 ( h_grid *parent, h_grid *child, h_amrp *amrp )
{
  int status;

  int i, j, r;
  int *inear_parent; /* indices of nearest parent points */
  int *inear_child;  /* indices of nearest child points */

  int rank = child->rank;
  int Lghost= child->Lghost;
  int Rghost= child->Rghost;
  int Nchild = child->N;

  H_DBL *xchild = h_get_grid_positions_wghosts ( child );
  H_DBL *xparent = h_get_grid_positions_wghosts ( parent );

  H_DBL *uchild, *uparent;
  
  H_DBL *x_to_interpolate;
  H_DBL *u_to_interpolate;

  x_to_interpolate = (H_DBL*) malloc ( 2*NPOINTS*sizeof( H_DBL ) ); 
  u_to_interpolate = (H_DBL*) malloc ( 2*NPOINTS*sizeof( H_DBL ) ); 

  
  /* update left ghost points */
  /* wskaznik i numeruje punkty siatki child ktorych wartosci mamy zinterpolowac */
  for (i = 0; i < Lghost; i++) {

      /* znaleziono indeksy punktów z któych będziemy interpolować */ 
      inear_parent = _h_find_5_nearest_indices ( xchild[i], parent, amrp );
      inear_child = _h_find_5_nearest_indices ( xchild[i], child, amrp );

      for (j = 0; j < NPOINTS; j++) {
          x_to_interpolate[j] = xparent[inear_parent[j]];
      }

      /* interpolujemy r-ty rząd */
      for (r = 0; r < rank; r++)
        {
            uchild = h_get_grid_values_wghosts ( child, r );
            uparent = h_get_grid_values_wghosts ( parent, r );

            for (j = 0; j < NPOINTS; j++) {
                u_to_interpolate[j] = uparent[inear_parent[j]];
            }

            gsl_interp_accel *acc
                = gsl_interp_accel_alloc ();
            
            gsl_spline *spline
                = gsl_spline_alloc (INTERP_TYPE, NPOINTS);
            
            gsl_spline_init (spline, x_to_interpolate, u_to_interpolate, NPOINTS);
            
                  
            uchild[i] =
                gsl_spline_eval (spline, xchild[i], acc);
          
          gsl_spline_free (spline);
          
          gsl_interp_accel_free (acc);
        }
      if ( inear_parent != NULL ) {
          free ( inear_parent );
      }
      if ( inear_child != NULL ) {
          free ( inear_child );
      }
  }


  /* update right ghost points */
  /* wskaznik i numeruje punkty siatki child ktorych wartosci mamy zinterpolowac */
  for (i = Nchild+Lghost; i < Nchild+Lghost+Rghost; i++) {

      if ( i == Nchild+Lghost+Rghost-1 ) {
          /* interpolujemy r-ty rząd */
          for (r = 0; r < rank; r++)
            {
                inear_parent = _h_find_5_nearest_indices ( xchild[i], parent, amrp );

                uchild = h_get_grid_values_wghosts ( child, r );
                uparent = h_get_grid_values_wghosts ( parent, r );
                /* uchild[i]=( uchild[i-1] + uchild[i-2] +6.0*uparent[inear_parent[3]] )/8.0; */
                uchild[i]=( -3.0*uchild[i-2] + 10.0*uchild[i-1] +8.0*uparent[inear_parent[3]] )/15.0;
            }
      }
      else {
      /* znaleziono indeksy punktów z któych będziemy interpolować */ 
      inear_parent = _h_find_5_nearest_indices ( xchild[i], parent, amrp );
      inear_child = _h_find_5_nearest_indices ( xchild[i], child, amrp );

      for (j = 0; j < NPOINTS; j++) {
          x_to_interpolate[j] = xparent[inear_parent[j]];
      }
      for (j = 0; j < NPOINTS; j++) {
          x_to_interpolate[NPOINTS+j] = xchild[inear_child[j]];
      }

      /* interpolujemy r-ty rząd */
      for (r = 0; r < rank; r++)
        {
            uchild = h_get_grid_values_wghosts ( child, r );
            uparent = h_get_grid_values_wghosts ( parent, r );

            for (j = 0; j < NPOINTS; j++) {
                u_to_interpolate[j] = uparent[inear_parent[j]];
            }
            for (j = 0; j < NPOINTS; j++) {
                u_to_interpolate[NPOINTS+j] = uchild[inear_child[j]];
            }

            gsl_interp_accel *acc
                = gsl_interp_accel_alloc ();
            
            gsl_spline *spline
                = gsl_spline_alloc (INTERP_TYPE, NPOINTS);
            
            gsl_spline_init (spline, x_to_interpolate, u_to_interpolate, NPOINTS);
            

            uchild[i] =
                gsl_spline_eval (spline, xchild[i], acc);

            /* child->u[i+r*(Nchild+Lghost+Rghost)] = */
            /*     gsl_spline_eval (spline, xchild[i], acc); */

            
          gsl_spline_free (spline);
          
          gsl_interp_accel_free (acc);
        }
      if ( inear_parent != NULL ) {
          free ( inear_parent );
      }
      if ( inear_child != NULL ) {
          free ( inear_child );
      }
      }
  }
}



int _h_update_glevel ( h_glevel *parent, h_glevel *child, h_amrp *amrp )
{
  VL((" *** Updating from child glevel l=%d to parent glevel l=%d\n",
      child->l, parent->l ));
  int status;
  int m, M;

  /* if ( _h_dbl_eq( child->grid[0]->t,  parent->grid[0]->t ) ) { */

      VL((" *** Updating from child glevel t=%e to parent glevel t=%e\n",
          child->grid[0]->t, parent->grid[0]->t));

      /* sleep(1); */
      M = child->M;
      for (m = 0; m < M; m++) {

          status = _h_update_grid_interior ( child->grid[m]->parent, child->grid[m], amrp );
          if ( status != H_OK )
              return status;
  
          status = _h_update_grid_ghosts_new_new ( child->grid[m]->parent, child->grid[m], amrp );
          if ( status != H_OK )
              break;
      }

  /* } */
  /* else exit(EXIT_FAILURE); */
  return status;
}
