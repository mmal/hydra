#include "ivp_1D.h"


void h_ivp_1D ( h_grid *g, void *params, int ord, ... )
{
  int i, j, N;

  va_list arguments;

  H_DBL (*fnc_ptr)( H_DBL, void * );

  if ( g == NULL || g->x == NULL || g->u == NULL )
      _STAT_MSG ( "Initial value problem: using given initial conditions",
                  "grid non allocated",
                  ERROR, 0 );

  if ( g->ord != ord )
      _STAT_MSG ( "Initial value problem: using given initial conditions",
                  "orders not equal",
                  ERROR, 0 );

  va_start ( arguments, ord );

  for ( i = 0; i < ord; i++ ) {
      fnc_ptr = va_arg ( arguments, H_DBL (*)(H_DBL, void*) );
      if ( fnc_ptr == NULL ) {
          va_end ( arguments );

          _STAT_MSG ( "Initial value problem: using given initial conditions",
                      "number of arguments is not equal to order",
                      ERROR, 0 );
      }
      else {
          N = g->N;

          for (j = 0; j < N; j++) {
              g->u[i*N+j] = fnc_ptr( g->x[j], params );
          }
      }
  }

  va_end ( arguments );

  _STAT_MSG ( "Initial value problem: using given initial conditions",
              NULL,
              OK, 0 );
}


void h_ivp_1D_grid_family ( h_grid *cg, h_amrp *p, void *params, int ord, ... )
{
  int i, j, l, N;

  int lmax = p->lmax;

  va_list arguments;

  H_DBL (*fnc_ptr)( H_DBL, void * );

  h_grid * g = cg;

  if ( g == NULL || g->x == NULL || g->u == NULL )
      _STAT_MSG ( "Initial value problem: using given initial conditions",
                  "grid non allocated",
                  ERROR, 0 );

  if ( g->ord != ord )
      _STAT_MSG ( "Initial value problem: using given initial conditions",
                  "orders not equal",
                  ERROR, 0 );

  va_start ( arguments, ord );

  for ( i = 0; i < ord; i++ ) {
      fnc_ptr = va_arg ( arguments, H_DBL (*)(H_DBL, void*) );
      if ( fnc_ptr == NULL ) {
          va_end ( arguments );
          _STAT_MSG ( "Initial value problem: using given initial conditions",
                      "number of arguments is not equal to order",
                      ERROR, 0 );
      }
      else {
          for (l = 0; l < lmax; l++) {
              N = g->N;
              for (j = 0; j < N; j++) {
                  g->u[i*N+j] = fnc_ptr( g->x[j], params );
              }
              g = (h_grid*) g->offspring;
          }
      }

      g = cg;
  }

  va_end ( arguments );

  _STAT_MSG ( "Initial value problem: using given initial conditions",
              NULL,
              OK, 0 );
}
