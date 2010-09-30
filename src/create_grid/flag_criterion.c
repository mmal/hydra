/**
 * @file   flag_criterion.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Tue Sep  7 16:41:24 2010
 * 
 * @brief  This file contains all available flagging criterion
 * functions.
 * 
 * 
 */


#include "flag_criterion.h"


int h_fc_Test ( h_grid *grid, h_amrp *amrp, h_fnc *f, H_DBL * tau )
{
  int i;
  
  int N = grid->N;

  for (i = 0; i < N; i++) {
      tau[i] = sin(2*i)*10;
  }
  
  return H_OK;
}

/* int jac_null (H_DBL t, const H_DBL y[], H_DBL *dfdy,  */
/*               H_DBL dfdt[], void *params) */
/* { */
/*   H_DBL temp; */
/*   void *vtemp; */

/*   temp = t; */
/*   temp = dfdy[0]; */
/*   temp = dfdt[0]; */
/*   temp = y[0]; */
/*   vtemp = params; */
  
/*   printf("This stepper type requires jacobian matrix\n");  */
/*   return 0; */
/* } */


/* int */
/* equation ( H_DBL t, const H_DBL y[], H_DBL f[], void *params ) */
/* { */
/*   int i, status; */
  
/*   h_hms * m = ( h_hms *) params; */

/*   int (*deriv_ptr)(H_DBL, H_DBL*, H_DBL*, H_DBL*, int, int, void*); */
/*   int N = m->g->Ntotal; */
/*   int ISN = m->f->ISN; */
/*   int Ncalls; */
  
/*   int ngh = m->p->ngh; */
/*   int sp = m->p->sp; */

  
/*   int Lghost = m->g->Lghost; */
/*   int Rghost = m->g->Rghost; */

/*   int Lmove, Rmove; */
  
/*   H_DBL h = m->g->h; */

/*   H_DBL xL_m, xR_m; */
  
/*   H_DBL *yptr = y; */

/*   H_DBL *xptr = m->g->x; */
  
/*   /\* h_grid *master_grid = h_alloc_grid ( ); *\/ */

  
/*   /\* if( t == m->g->tlast+m->g->dt/2 && t == m->g->tlast + m->g->dt ) *\/ */
/*   /\*   { *\/ */
/*   /\*       m->g->Ncalls++; *\/ */
/*   /\*       Ncalls = m->g->Ncalls; *\/ */
/*   /\*   } *\/ */

/*   ( m->g->Ncalls++ ); */
  
/*   Ncalls = m->g->Ncalls; */

/*   if ( Ncalls == 4 ) { */
/*       m->g->tlast = t; */
/*       m->g->Ncalls = 0; */
/*   } */

  
/*   /\* printf(" wyw. p. s. r. dt=%e" *\/ */
/*   /\*        ", stepper %s, order=%u, t=%f, Ncalls=%d\n", m->g->dt, *\/ */
/*   /\*        m->f->step_T->name, (*m->f->step_T->order)(NULL), t, Ncalls ); *\/ */
/*   /\* sleep( 0 ); *\/ */
  
/*   if ( m->g->is_master == H_TRUE ) */
/*     { */
/*         for (i = 0; i < N; i++) { */
/*             if (i<ISN) { */
/*                 deriv_ptr = m->f->deriv[i]; */
/*                 status = deriv_ptr(t, m->g->x, yptr, f, i, N, &h); */
/*             } */
/*             else if ( i>N-ISN ) { */
/*                 status = (*m->f->deriv[N-1-i])(t, m->g->x, yptr, f, -i, N, &h); */
/*             } */
/*             else  */
/*                 status = (*m->f->deriv[ISN-1])(t, m->g->x, yptr, f, i, N, &h); */
/*         } */
/*     } */
/*   else { */
/*       /\* VL(("  Grid is not master\n")); *\/ */

/*       /\* master_grid = (h_grid *) m->g->master; *\/ /\* TODO: *\/ */

/*       /\* VL(("  Grid is not master 1a\n")); *\/ */

/*       /\* xL_m = master_grid->xL; *\/ */
/*       /\* xR_m = master_grid->xR; *\/ */
/*       xL_m = -1.; */
/*       xR_m = 1.; */
      
/*       /\* VL(("  Grid is not master 1b\n")); *\/ */

/*       if ( Lghost < ngh - (Ncalls-1)*sp  ) */
/*           Lmove = 0; */
/*       else */
/*           Lmove = (Ncalls-1)*sp; */
      
/*       if ( Rghost < ngh - (Ncalls-1)*sp  ) */
/*           Rmove = 0; */
/*       else */
/*           Rmove = (Ncalls-1)*sp; */

/*       N = N-Lmove-Rmove; */

/*       /\* VL(("  Grid is not master 2, Lmove=%d, Rmove=%d\n", Lmove, Rmove)); *\/ */
/*       /\* VL(("  Grid is not master 3, Lghost=%d, Rghost=%d\n", Lghost, Rghost)); *\/ */
/*       /\* VL(("  Grid is not master 4, x[0]=%f, x[N-1]=%f\n", xptr[0], xptr[N-1])); *\/ */
/*       /\* yptr+=Lmove; *\/ */
/*       /\* xptr+=Lmove; *\/ */

/*       /\* for (i = Lmove; i < N; i++) { *\/ */
/*       /\*     /\\* VL(("i=%d\n", i)); *\\/ *\/ */
/*       /\*     if ( i<ISN && ( xptr[i] == xL_m + i*h ) ) { *\/ */
/*       /\*         status = (*m->f->deriv[i])(t, xptr, yptr, f, i, N, &h); *\/ */
/*       /\*     } *\/ */
/*       /\*     else if ( i>N-ISN && ( xptr[i] == xR_m - i*h ) ) { *\/ */
/*       /\*         status = (*m->f->deriv[ISN-1-i])(t, xptr, yptr, f, -i, N, &h); *\/ */
/*       /\*     } *\/ */
/*       /\*     else *\/ */
/*       /\*         status = (*m->f->deriv[ISN-1])(t, xptr, yptr, f, i, N, &h); *\/ */
/*       /\* } *\/ */

/*       for (i = Lmove+1; i < N; i++) { */
          
/*           if ( xptr[i]-2*h >= xL_m && xptr[i]+2*h <= xR_m ) { */
/*               /\* centered derivative *\/ */
/*               status = (*m->f->deriv[2])(t, xptr, yptr, f, i, N, &h); */
/*           } */
/*           else if ( xptr[i] - h == xL_m ) { */
/*               status = (*m->f->deriv[1])(t, xptr, yptr, f, i, N, &h); */
/*           } */
/*           else if ( xptr[i] + h == xR_m ) { */
/*               status = (*m->f->deriv[1])(t, xptr, yptr, f, -i, N, &h); */
/*           } */
/*           else if ( xptr[i] == xL_m ) { */
/*               status = (*m->f->deriv[0])(t, xptr, yptr, f, i, N, &h); */
/*           } */
/*           else if ( xptr[i] == xR_m ) { */
/*               status = (*m->f->deriv[0])(t, xptr, yptr, f, -i, N, &h); */
/*           } */
/*       } */
      
/*       for (i = 0; i < Lmove; i++) { */
/*           yptr[i] = 0.; */
/*       } */
/*       for (i = N+Lmove; i < N + Lmove + Rmove; i++) { */
/*           yptr[i] = 0.; */
/*       } */
      
/*       /\* exit(0); *\/ */
/*   } */

/*   /\* h_free_grid ( master_grid ); *\/ */
  
/*   return GSL_SUCCESS; */
/* } */


/* h_hms *_condense_grid ( h_hms *m, int factor ) */
/* { */
/*   h_hms * mfine = h_alloc_hms ( ); */

/*   int Lghost, Rghost; */
  
/*   mfine->f=m->f; */
/*   mfine->p=m->p; */

  
/*   if ( m->g->Lghost < (m->p->ngh)/factor ) */
/*       Lghost = (m->g->Lghost)*factor; */
/*   else */
/*       Lghost = m->p->ngh; */

/*   if ( m->g->Rghost < (m->p->ngh)/factor ) */
/*       Rghost = (m->g->Rghost)*factor; */
/*   else */
/*       Rghost = m->p->ngh; */
  
/*   h_init_grid ( mfine->g, m->g->xL, m->g->xR, */
/*                 m->g->N*factor, Lghost, Rghost, */
/*                 m->g->rank, m->g->l, m->g->m ); */

/*   mfine->g->is_master = m->g->is_master; */

/*   mfine->g->dt = m->g->dt/factor; */
  
/*   if ( m->g->t == 0.0 ) */
/*       _h_acd_to_one_grid ( mfine->g, mfine->f ); */
/*   else { /\* interpolation is needed *\/ */
/*       printf( "interpolation is needed\n\n\n"); */
/*       exit(0); */
/*   } */

/*   return mfine; */
/* } */


/* int _h_fc_integrate ( H_DBL t0, H_DBL t1, H_DBL dt, H_DBL *u, h_hms *m ) */
/* { */
/*   int status; */
  
/*   int rank = m->g->rank; */
  
/*   int N = m->g->Ntotal; */
  
/*   gsl_odeiv_step * s */
/*       = gsl_odeiv_step_alloc ( m->f->step_T, rank*N); */

     
/*   gsl_odeiv_system sys = {equation, jac_null, rank*N, m}; */

/*   H_DBL * dydt_in  =  ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) ); */
/*   H_DBL * dydt_out =  ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) ); */
/*   H_DBL * y_err = ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) ); */

/*   /\* initialise dydt_in from system parameters *\/ */
/*   /\* GSL_ODEIV_FN_EVAL(&sys, t0, u, dydt_in); *\/ */
  
/*   while (t0 < t1) { */
/*       /\* VL(("Integrating\n")); *\/ */
/*       status = gsl_odeiv_step_apply ( s, t0, dt, */
/*                                       u, y_err, */
/*                                       NULL, */
/*                                       NULL, */
/*                                       &sys ); */
      
/*       if (status != GSL_SUCCESS) { */
/*           printf("ERROR: status != GSL_SUCCESS\n"); */
/*           break; */
/*       } */

/*       /\* memcpy( dydt_in, dydt_out, (rank*N)*sizeof(H_DBL) ); *\/ */
     
/*       t0 += dt; */
/*       /\* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "main integrated rank 0", 2 ); *\/ */
/*   } */
  
/*   gsl_odeiv_step_free ( s ); */

/*   return status; */
/* } */


/* int h_fc_Richardson ( void *vm, H_DBL * tau ) */
/* { */
/*   h_hms *m = (h_hms *) vm; */

  
/*   int i, status; */
/*   int rank = m->g->rank; */
/*   int factor = 2; */

/*   const int q=4; */
  
/*   h_hms *mfine = _condense_grid ( m, factor ); */
  
/*   H_DBL dt = (m->g->h)/(m->p->lmbd); */
/*   H_DBL t0 = m->g->t; */
/*   H_DBL t1 = t0+dt; */

  
/*   H_DBL * u1  = ( H_DBL* ) malloc ( rank*m->g->Ntotal*sizeof( H_DBL ) ); */
    
/*   H_DBL * u2  = ( H_DBL* ) malloc ( rank*mfine->g->Ntotal*sizeof( H_DBL ) ); */


/*   memcpy( u1, m->g->u, (rank*m->g->Ntotal)*sizeof(H_DBL) ); */
    
/*   status = _h_fc_integrate ( t0, t1, dt, u1, m ); */

/*   VL(("Integrated l=%d, m=%d\n", m->g->l, m->g->m)); */
  
/*   memcpy( u2, mfine->g->u, (rank*mfine->g->Ntotal)*sizeof(H_DBL) ); */

/*   VL(("Before Integrating l=%d, m=%d fine\n", mfine->g->l, mfine->g->m)); */
    
/*   status = _h_fc_integrate ( t0, t1, dt/factor, u2, mfine ); */

/*   VL(("Integrated l=%d, m=%d fine\n", mfine->g->l, mfine->g->m)); */

/*   for (i = 0; i < m->g->N; i++) { */
/*       tau[i] = fabs( (u2[2*i+m->g->Lghost] */
/*                       - u1[i+m->g->Lghost])/(pow(2., q)-1.)/2. ); */
/*       printf("  tau[%d] = %e\n", i, tau[i]); */
/*   } */
  
/*   return H_OK; */
/* } */


/* /\* TODO: *\/ */
/* /\* int h_fc_Energy ( void *vm, H_DBL * tau ){ *\/ */
/* /\*   return H_ER; *\/ */
/* /\* } *\/ */

/* int h_fc_SV ( void *vm, H_DBL * tau ) */
/* { */
/*   h_hms *m = (h_hms *) vm; */

/*   int i, N = m->g->N; */

/*   H_DBL *u = h_get_grid_values ( m->g, 0 ); */
/*   H_DBL h = m->g->h; */

/*   for (i = 2; i < N-2; i++) { */
/*       tau[i] = fabs ( ( u[i-2]-8*u[i-1]+8*u[i+1]-u[i+2] )/( 12*h ) ); */
/*   } */

/*   tau[0] = 0.; */
/*   tau[1] = 0.; */
/*   tau[N-2] = 0.; */
/*   tau[N-1] = 0.; */

/*   return H_OK; */
/* } */

/* /\* int h_fc_TV ( void *vm, H_DBL * tau ){ *\/ */
/* /\*   return H_ER; *\/ */
/* /\* } *\/ */
