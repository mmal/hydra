

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
  
  h_hms * m = ( h_hms *) params;

  int (*deriv_ptr)(H_DBL, H_DBL*, H_DBL*, H_DBL*, int, int, void*);
  int N = m->g->Ntotal;
  int ISN = m->f->ISN;
  int Ncalls;
  
  int ngh = m->p->ngh;
  int sp = m->p->sp;

  
  int Lghost = m->g->Lghost;
  int Rghost = m->g->Rghost;

  int Lmove, Rmove;
  
  H_DBL h = m->g->h;

  H_DBL xL_m, xR_m;
  
  H_DBL *yptr = y;

  H_DBL *xptr = m->g->x;
  
  /* h_grid *master_grid = h_alloc_grid ( ); */

  
  /* if( t == m->g->tlast+m->g->dt/2 && t == m->g->tlast + m->g->dt ) */
  /*   { */
  /*       m->g->Ncalls++; */
  /*       Ncalls = m->g->Ncalls; */
  /*   } */

  ( m->g->Ncalls++ );
  
  Ncalls = m->g->Ncalls;

  if ( Ncalls == 4 ) {
      m->g->tlast = t;
      m->g->Ncalls = 0;
  }

  
  /* printf(" wyw. p. s. r. dt=%e" */
  /*        ", stepper %s, order=%u, t=%f, Ncalls=%d\n", m->g->dt, */
  /*        m->f->step_T->name, (*m->f->step_T->order)(NULL), t, Ncalls ); */
  /* sleep( 0 ); */
  
  if ( m->g->is_master == H_TRUE )
    {
        for (i = 0; i < N; i++) {
            if (i<ISN) {
                deriv_ptr = m->f->deriv[i];
                status = deriv_ptr(t, m->g->x, yptr, f, i, N, &h);
            }
            else if ( i>N-ISN ) {
                status = (*m->f->deriv[N-1-i])(t, m->g->x, yptr, f, -i, N, &h);
            }
            else 
                status = (*m->f->deriv[ISN-1])(t, m->g->x, yptr, f, i, N, &h);
        }
    }
  else {
      /* VL(("  Grid is not master\n")); */

      /* master_grid = (h_grid *) m->g->master; */ /* TODO: */

      /* VL(("  Grid is not master 1a\n")); */

      /* xL_m = master_grid->xL; */
      /* xR_m = master_grid->xR; */
      xL_m = -1.;
      xR_m = 1.;
      
      /* VL(("  Grid is not master 1b\n")); */

      if ( Lghost < ngh - (Ncalls-1)*sp  )
          Lmove = 0;
      else
          Lmove = (Ncalls-1)*sp;
      
      if ( Rghost < ngh - (Ncalls-1)*sp  )
          Rmove = 0;
      else
          Rmove = (Ncalls-1)*sp;

      N = N-Lmove-Rmove;

      /* VL(("  Grid is not master 2, Lmove=%d, Rmove=%d\n", Lmove, Rmove)); */
      /* VL(("  Grid is not master 3, Lghost=%d, Rghost=%d\n", Lghost, Rghost)); */
      /* VL(("  Grid is not master 4, x[0]=%f, x[N-1]=%f\n", xptr[0], xptr[N-1])); */
      /* yptr+=Lmove; */
      /* xptr+=Lmove; */

      /* for (i = Lmove; i < N; i++) { */
      /*     /\* VL(("i=%d\n", i)); *\/ */
      /*     if ( i<ISN && ( xptr[i] == xL_m + i*h ) ) { */
      /*         status = (*m->f->deriv[i])(t, xptr, yptr, f, i, N, &h); */
      /*     } */
      /*     else if ( i>N-ISN && ( xptr[i] == xR_m - i*h ) ) { */
      /*         status = (*m->f->deriv[ISN-1-i])(t, xptr, yptr, f, -i, N, &h); */
      /*     } */
      /*     else */
      /*         status = (*m->f->deriv[ISN-1])(t, xptr, yptr, f, i, N, &h); */
      /* } */

      for (i = Lmove; i < N; i++) {
          
          if ( xptr[i]-2*h >= xL_m && xptr[i]+2*h <= xR_m ) {
              /* centered derivative */
              status = (*m->f->deriv[2])(t, xptr, yptr, f, i, m->g->Ntotal, &h);
          }
          else if ( xptr[i] - h == xL_m ) {
              status = (*m->f->deriv[1])(t, xptr, yptr, f, i, m->g->Ntotal, &h);
          }
          else if ( xptr[i] + h == xR_m ) {
              status = (*m->f->deriv[1])(t, xptr, yptr, f, -i, m->g->Ntotal, &h);
          }
          else if ( xptr[i] == xL_m ) {
              status = (*m->f->deriv[0])(t, xptr, yptr, f, i, m->g->Ntotal, &h);
          }
          else if ( xptr[i] == xR_m ) {
              status = (*m->f->deriv[0])(t, xptr, yptr, f, -i, m->g->Ntotal, &h);
          }
      }
      
      /* for (i = 0; i < Lmove; i++) { */
      /*     yptr[i] = 66.; */
      /* } */
      /* for (i = N+Lmove; i < N + Lmove + Rmove; i++) { */
      /*     yptr[i] = 66.; */
      /* } */
      
      /* exit(0); */
  }

  /* h_free_grid ( master_grid ); */
  
  return GSL_SUCCESS;
}


int _h_step ( H_DBL t0, H_DBL t1, H_DBL dt,
              H_DBL *u, h_hms *m )
{
  int status;
  
  int rank = m->g->rank;
  
  int N = m->g->Ntotal;
  
  gsl_odeiv_step * s
      = gsl_odeiv_step_alloc ( m->f->step_T, rank*N);

     
  gsl_odeiv_system sys = {RHS_eq, RHS_jac, rank*N, m};

  H_DBL * dydt_in  =  ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) );
  H_DBL * dydt_out =  ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) );
  H_DBL * y_err = ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) );

  /* initialise dydt_in from system parameters */
  /* GSL_ODEIV_FN_EVAL(&sys, t0, u, dydt_in); */
  
  while (t0 < t1)
    {
        status = gsl_odeiv_step_apply ( s, t0, dt, u, y_err,
                                        NULL, NULL, &sys );
        
      if ( status != GSL_SUCCESS ) {
          _STAT_MSG("Stepping _h_step", "status != GSL_SUCCESS",
                    H_WA, 0);
          break;
      }

      /* memcpy( dydt_in, dydt_out, (rank*N)*sizeof(H_DBL) ); */
     
      t0 += dt;
      /* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "main integrated rank 0", 2 ); */
  }
  
  gsl_odeiv_step_free ( s );

  free ( dydt_in );
  free ( dydt_out );
  free ( y_err );

  return status;
}
