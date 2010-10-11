

#include "step.h"


int
RHS_jac ( H_DBL t, const H_DBL y[], H_DBL *dfdy, 
           H_DBL dfdt[], void *params )
{
  H_DBL temp;
  void *vtemp;

  temp = t;
  temp = dfdy[0];
  temp = dfdt[0];
  temp = y[0];
  vtemp = params;
  
  _STAT_MSG("Jacobian call", "This stepper requires Jacobian",
            H_ER, 0);
  
  return H_ER;
}


int
RHS_eq ( H_DBL t, const H_DBL y[], H_DBL f[], void *params )
{
  int i, status;
   
  h_hss *hss = (h_hss *) params;

  int (*deriv_ptr)(H_DBL, H_DBL*, H_DBL*, H_DBL*, int, int, void*);
  int N = hss->grid->Ntotal;
  int Ntotal = hss->grid->Ntotal;
  int ISN = hss->fnc->ISN;
  int Ncalls;
  
  int ngh = hss->amrp->ngh;
  int sp = hss->amrp->sp;
  
  int Lghost = hss->grid->Lghost;
  int Rghost = hss->grid->Rghost;

  int Lmove, Rmove;
  
  H_DBL h = hss->grid->h;
  H_DBL xL_m, xR_m;
  
  H_DBL *yptr = y;
  H_DBL *xptr = hss->grid->x;
  

  ( hss->grid->Ncalls++ );
  
  Ncalls = hss->grid->Ncalls;

  if ( Ncalls == 4 ) {
      hss->grid->tlast = t;
      hss->grid->Ncalls = 0;
  }

  
  /* printf(" wyw. p. s. r. dt=%e" */
  /*        ", stepper %s, order=%u, t=%f, Ncalls=%d\n", hss->grid->dt, */
  /*        hss->fnc->step_T->name, (*hss->fnc->step_T->order)(NULL), t, Ncalls ); */
  /* printf("l=%d  Ncalls=%d\n", hss->grid->l, Ncalls); */
  /* sleep( 1 ); */
  
  if ( hss->grid->is_master == H_TRUE )
    {
        for (i = 0; i < N; i++) {
            if ( i == 0 ) {
                status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
            }
            else if ( i == 1 ) {
                status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
            }
            else if ( i == Ntotal-1 ) {
                status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h);
            }
            else if ( i == Ntotal-2 ) {
                status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h);
            }
            else {
                /* centered derivative */
                status = (*hss->fnc->deriv[2])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
            }
        }
        /* for (i = 0; i < N; i++) { */
        /*     if (i<ISN) { */
        /*         deriv_ptr = hss->fnc->deriv[i]; */
        /*         status = deriv_ptr(t, hss->grid->x, yptr, f, i, N, &h); */
        /*     } */
        /*     else if ( i>N-ISN ) { */
        /*         status = (*hss->fnc->deriv[N-1-i])(t, hss->grid->x, yptr, f, -i, N, &h); */
        /*     } */
        /*     else */
        /*         status = (*hss->fnc->deriv[ISN-1])(t, hss->grid->x, yptr, f, i, N, &h); */
        /* } */
    }
  else {
      /* VL(("  Grid is not master\n")); */

      xL_m = -1.;
      xR_m = 1.;
      
      if ( Lghost < ngh - (Ncalls-1)*sp  )
          Lmove = 0;
      else
          Lmove = (Ncalls-1)*sp;
      
      if ( Rghost < ngh - (Ncalls-1)*sp  )
          Rmove = 0;
      else
          Rmove = (Ncalls-1)*sp;

      /* N = N-Lmove-Rmove; */

      /* VL(("  Grid is not master 2, Lmove=%d, Rmove=%d\n", Lmove, Rmove)); */
      /* VL(("  Grid is not master 3, Lghost=%d, Rghost=%d\n", Lghost, Rghost)); */
      /* VL(("  Grid is not master 4, x[0]=%f, x[N-1]=%f\n", xptr[0], xptr[N-1])); */
      /* yptr+=Lmove; */
      /* xptr+=Lmove; */

      /* for (i = Lmove; i < N; i++) { */
      /*     /\* VL(("i=%d\n", i)); *\/ */
      /*     if ( i<ISN && ( xptr[i] == xL_m + i*h ) ) { */
      /*         status = (*hss->fnc->deriv[i])(t, xptr, yptr, f, i, N, &h); */
      /*     } */
      /*     else if ( i>N-ISN && ( xptr[i] == xR_m - i*h ) ) { */
      /*         status = (*hss->fnc->deriv[ISN-1-i])(t, xptr, yptr, f, -i, N, &h); */
      /*     } */
      /*     else */
      /*         status = (*hss->fnc->deriv[ISN-1])(t, xptr, yptr, f, i, N, &h); */
      /* } */

      for (i = Lmove; i < N-Rmove; i++) {

          /* printf("x[%d]=%e\n", i, xptr[i]); */
          /* printf("u[%d]=%e\n", i, y[i]); */
          /* VL(("i=%d\n", i)); */
          
          /* if ( xptr[i] - h == xL_m ) { */
          /*     /\* VL(("xptr[i] - h == xL_m ")); *\/ */
          /*     status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h); */
          /* } */
          /* else if ( xptr[i] + h == xR_m ) { */
          /*     /\* VL(("xptr[i] + h == xR_m ")); *\/ */
          /*     status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h); */
          /* } */
          /* else if ( xptr[i] == xL_m ) { */
          /*     /\* VL(("xptr[i] == xL_m ")); *\/ */
          /*     status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h); */
          /* } */
          /* else if ( xptr[i] == xR_m ) { */
          /*     /\* VL(("xptr[i] == xR_m ")); *\/ */
          /*     status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h); */
          /* } */
          /* else if ( xptr[i]-2*h >= xL_m && xptr[i]+2*h <= xR_m ) { */
          /*     /\* centered derivative *\/ */
          /*     /\* VL(("xptr[i]-2*h >= xL_m && xptr[i]+2*h <= xR_m ")); *\/ */
          /*     status = (*hss->fnc->deriv[2])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h); */
          /* } */


          if ( i == 0 ) {
              status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
          }
          else if ( i == 1 ) {
              status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
          }
          else if ( i == Ntotal-1 ) {
              status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h);
          }
          else if ( i == Ntotal-2 ) {
              status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h);
          }
          else {
              /* centered derivative */
              status = (*hss->fnc->deriv[2])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
          }
          

          /* sleep(1); */

      }
      
  }
  
  return GSL_SUCCESS;
}



int _h_boialg_step_glevel ( h_glevel *glevel, h_amrp *amrp, h_fnc *fnc )
{
  char *fnc_msg = "Berger&Oliger step glevel";

  int m, M, status;

  h_grid *grid;

  if ( glevel == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_glevel is unallocated",
                  WARNING, 0 );
      return H_ER;
  }
  else if ( glevel->grid == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_grid* is unallocated",
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
      M = glevel->M;
      
      for (m = 0; m < M; m++) {

          grid = glevel->grid[m];

          status = _h_boialg_step_grid ( grid, amrp, fnc );
          
          if ( status != H_OK )
              break;
      }
      
      return status;
  }
}



int _h_boialg_step_grid ( h_grid *grid, h_amrp *amrp, h_fnc *fnc )
{
  char *fnc_msg = "Berger&Oliger step grid";

  int rank, N, status;

  H_DBL t0, t1, dt, *u_err, *u;

  h_hss *hss = (h_hss*) malloc ( sizeof( h_hss ) );
  
  /* h_hss *hss; */
  gsl_odeiv_step *s;
  gsl_odeiv_system sys;

  
  if ( grid == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_grid is unallocated",
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
      hss->grid = grid;
      hss->amrp = amrp;
      hss->fnc = fnc;

      rank = grid->rank;
      N = grid->Ntotal;
      
      s = gsl_odeiv_step_alloc ( gsl_odeiv_step_rk4, rank*N);
      
      /* sys.function = {RHS_eq, RHS_jac, rank*N, hss}; */
      sys.function = RHS_eq;
      sys.jacobian = RHS_jac;
      sys.dimension = rank*N;
      sys.params = hss;

      
      u_err = ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) );
      if ( u_err == NULL ) {
          return H_ER;
      }

      u = grid->u;
      
      t0 = grid->t;
      dt = grid->dt;
      t1 = t0 + dt;

      /* printf("TEST\n"); */
      /* printf("dt=%e\n", dt); */
      
      while ( t0 < t1 )
        {
            status = gsl_odeiv_step_apply ( s, t0, dt, grid->u, u_err,
                                            NULL, NULL, &sys );
            
            if ( status != GSL_SUCCESS ) {
                _STAT_MSG("Stepping _h_step", "status != GSL_SUCCESS",
                          WARNING, 0);
                break;
            }
            
            /* memcpy( dydt_in, dydt_out, (rank*N)*sizeof(H_DBL) ); */
            
            t0 += dt;
            grid->t += dt;
            
            /* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "main integrated rank 0", 2 ); */
        }
      
      gsl_odeiv_step_free ( s );

      if ( u_err != NULL )
          free ( u_err );

      if ( hss != NULL )
          free ( hss );
      
      return H_OK;
  }
}


/* int _h_step ( H_DBL t0, H_DBL t1, H_DBL dt, */
/*               H_DBL *u, h_hms *m ) */
/* { */
/*   int status; */
  
/*   int rank = m->g->rank; */
  
/*   int N = m->g->Ntotal; */
  
/*   gsl_odeiv_step * s */
/*       = gsl_odeiv_step_alloc ( m->f->step_T, rank*N); */

     
/*   gsl_odeiv_system sys = {RHS_eq, RHS_jac, rank*N, m}; */

/*   H_DBL * dydt_in  =  ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) ); */
/*   H_DBL * dydt_out =  ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) ); */
/*   H_DBL * y_err = ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) ); */

/*   /\* initialise dydt_in from system parameters *\/ */
/*   /\* GSL_ODEIV_FN_EVAL(&sys, t0, u, dydt_in); *\/ */
  
/*   while (t0 < t1) */
/*     { */
/*         status = gsl_odeiv_step_apply ( s, t0, dt, u, y_err, */
/*                                         NULL, NULL, &sys ); */
        
/*       if ( status != GSL_SUCCESS ) { */
/*           _STAT_MSG("Stepping _h_step", "status != GSL_SUCCESS", */
/*                     H_WA, 0); */
/*           break; */
/*       } */

/*       /\* memcpy( dydt_in, dydt_out, (rank*N)*sizeof(H_DBL) ); *\/ */
     
/*       t0 += dt; */
/*       /\* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "main integrated rank 0", 2 ); *\/ */
/*   } */
  
/*   gsl_odeiv_step_free ( s ); */

/*   free ( dydt_in ); */
/*   free ( dydt_out ); */
/*   free ( y_err ); */

/*   return status; */
/* } */
