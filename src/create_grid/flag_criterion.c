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


int h_fc_Test ( void *vm, H_DBL * tau )
{
  int i ,N;

  h_hms *m = (h_hms *)vm;

  printf("\nh_fc_Test\n\n");
  
  N = m->g->N;

  for (i = 0; i < N; i++) {
      tau[i] = sin(2*i)*10;
  }
  
  return H_OK;
}

int jac_null (H_DBL t, const H_DBL y[], H_DBL *dfdy, 
              H_DBL dfdt[], void *params)
{
  H_DBL temp;
  void *vtemp;

  temp = t;
  temp = dfdy[0];
  temp = dfdt[0];
  temp = y[0];
  vtemp = params;
  
  printf("This stepper type requires jacobian matrix\n"); 
  return 0;
}

int equation ( H_DBL t, const H_DBL y[], H_DBL f[], void *params )
{
  int i, status;
  
  h_hms * m = ( h_hms *) params;

  int (*deriv_ptr)(H_DBL, H_DBL*, H_DBL*, H_DBL*, int, int, void*);
  int N = m->g->Ntotal;
  int ISN = m->f->ISN;
  H_DBL h = m->g->h;
  
  H_DBL *yptr = y;

  printf(" wywołana prawa strona równania"
         ", stepper %s, order=%u, t=%f, ISN=%d, N=%d\n", m->f->step_T->name, (*m->f->step_T->order)(NULL), t, ISN, N );
  sleep( 0 );
  
  if ( m->g->is_master == H_TRUE )
    {
        for (i = 0; i < N; i++) {
            if (i<ISN) {
                deriv_ptr = m->f->deriv[i];
                status = (*m->f->deriv[i])(t, m->g->x, yptr, f, i, N, &h);
            }
            else if ( i>N-ISN ) {
                status = (*m->f->deriv[N-1-i])(t, m->g->x, yptr, f, -i, N, &h);
            }
            else 
                status = (*m->f->deriv[ISN-1])(t, m->g->x, yptr, f, i, N, &h);
        }
    }
  else {
      printf("  Grid is not master\n\n\n!");
      exit(0);
  }
  
  return GSL_SUCCESS;
}


h_hms *_condense_grid ( h_hms *m, int factor )
{
  h_hms * mfine = h_alloc_hms ( );

  int Lghost, Rghost;
  
  mfine->f=m->f;
  mfine->p=m->p;

  
  if ( m->g->Lghost < (m->p->ngh)/factor )
      Lghost = (m->g->Lghost)*factor;
  else
      Lghost = m->p->ngh;

  if ( m->g->Rghost < (m->p->ngh)/factor )
      Rghost = (m->g->Rghost)*factor;
  else
      Rghost = m->p->ngh;
  
  h_init_grid ( mfine->g, m->g->xL, m->g->xR,
                m->g->N*factor, Lghost, Rghost,
                m->g->rank, m->g->l, m->g->m );

  mfine->g->is_master = m->g->is_master;
  
  if ( m->g->t == 0.0 )
      _h_acd_to_one_grid ( mfine->g, mfine->f );
  else { /* interpolation is needed */
      printf( "interpolation is needed\n\n\n");
      exit(0);
  }

  return mfine;
}


int _h_fc_integrate ( H_DBL t0, H_DBL t1, H_DBL dt, H_DBL *u, h_hms *m )
{
  int status;
  
  int rank = m->g->rank;
  
  int N = m->g->Ntotal;
  
  gsl_odeiv_step * s
      = gsl_odeiv_step_alloc ( m->f->step_T, rank*N);

     
  gsl_odeiv_system sys = {equation, jac_null, rank*N, m};

  H_DBL * dydt_in  = ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) );
  H_DBL * dydt_out = ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) );
  H_DBL * y_err = ( H_DBL* ) malloc( rank*N*sizeof( H_DBL ) );

  /* initialise dydt_in from system parameters */
  GSL_ODEIV_FN_EVAL(&sys, t0, u, dydt_in);
     
  while (t0 < t1) {
      VL(("Integrating\n"));
      status = gsl_odeiv_step_apply ( s, t0, dt,
                                      u, y_err,
                                      dydt_in,
                                      dydt_out,
                                      &sys );
      
      if (status != GSL_SUCCESS) {
          printf("ERROR: status != GSL_SUCCESS\n");
          break;
      }

      memcpy( dydt_in, dydt_out, (rank*N)*sizeof(H_DBL) );
     
      t0 += dt;
  }
  
  gsl_odeiv_step_free ( s );
  
  return status;
}


int h_fc_Richardson ( void *vm, H_DBL * tau )
{
  h_hms *m = (h_hms *) vm;

  
  int i, status;
  int rank = m->g->rank;
  int factor = 2;

  const int q=4;
  
  h_hms *mfine = _condense_grid ( m, factor );
  
  H_DBL dt = (m->g->h)/(m->p->lmbd);
  H_DBL t0 = m->g->t;
  H_DBL t1 = t0+dt;

  
  H_DBL * u1  = ( H_DBL* ) malloc ( rank*m->g->Ntotal*sizeof( H_DBL ) );
    
  H_DBL * u2  = ( H_DBL* ) malloc ( rank*mfine->g->Ntotal*sizeof( H_DBL ) );


  memcpy( u1, m->g->u, (rank*m->g->Ntotal)*sizeof(H_DBL) );
    
  status = _h_fc_integrate ( t0, t1, dt, u1, m );

  memcpy( u2, mfine->g->u, (rank*mfine->g->Ntotal)*sizeof(H_DBL) );

  status = _h_fc_integrate ( t0, t1, dt/factor, u2, mfine );

  for (i = m->g->Lghost; i < m->g->N+m->g->Lghost; i++) {
      tau[i] = fabs( (u2[2*i]-u1[i])/(pow(2., q)-1.)/2. );
      printf("  tau[%d] = %e\n", i, tau[i]);
  }
  
  return H_OK;
}


/* TODO: */
/* int h_fc_Energy ( void *vm, H_DBL * tau ){ */
/*   return H_ER; */
/* } */

/* int h_fc_SV ( void *vm, H_DBL * tau ){ */
/*   return H_ER; */
/* } */

/* int h_fc_TV ( void *vm, H_DBL * tau ){ */
/*   return H_ER; */
/* } */
