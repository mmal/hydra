

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
  int N = hss->grid->N;
  int Ntotal = hss->grid->Ntotal;
  int ISN = hss->fnc->ISN;
  int Ncalls;
  
  int ngh = hss->amrp->ngh;
  int sp = hss->amrp->sp;

  int rr = hss->amrp->rr;
  
  int Lghost = hss->grid->Lghost;
  int Rghost = hss->grid->Rghost;

  int Lmove, Rmove;
  
  H_DBL h = hss->grid->h;
  H_DBL xL_m, xR_m;
  
  H_DBL *yptr = y;
  H_DBL *xptr = hss->grid->x;
  

  ( hss->grid->Ncalls++ );
  
  Ncalls = hss->grid->Ncalls;

  if ( Ncalls == 4*rr ) {
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
        for (i = 0; i < Ntotal; i++) {
            if ( i == 0 ) {
                status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
            }
            else if ( i == 1 ) {
                status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
            }
            else if ( i == Ntotal-2 ) {
                status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h);
            }
            else if ( i == Ntotal-1 ) {
                status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
            }
            else {
                /* centered derivative */
                status = (*hss->fnc->deriv[2])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
            }
        }
    }
  else {
      /* VL(("  Grid is not master\n")); */

      /* /\* TODO: *\/ */
      /* xL_m = -1.0; */
      /* xR_m = 1.0; */


      if ( Lghost <= ngh )
          Lmove = 0;
      else if ( Lghost <= ngh - (Ncalls-1)*sp  ) /* \bug (Ncalls-0) ? */
          Lmove = 0;
      else
          Lmove = (Ncalls-0)*sp;
      
      if ( Rghost <= ngh - (Ncalls-1)*sp  ) /* \bug (Ncalls-0) ? */
          Rmove = 0;
      else
          Rmove = (Ncalls-0)*sp;

      /* printf("m=%d, Ncalls=%d, Lmove=%d, Rmove=%d\n", */
      /* hss->grid->m, Ncalls, Lmove, Rmove); */
      /* sleep( 2 ); */
      
      for (i = Lmove; i < Ntotal-Rmove-Lmove; i++) {

          if ( i == 0 ) {
              status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
          }
          else if ( i == 1 ) {
              status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, i, hss->grid->Ntotal, &h);
          }
          else if ( i == Ntotal-2 ) {
              status = (*hss->fnc->deriv[1])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h);
          }
          else if ( i == Ntotal-1 ) {
              status = (*hss->fnc->deriv[0])(t, xptr, yptr, f, -i, hss->grid->Ntotal, &h);
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
