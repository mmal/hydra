

#include "fnc_common.h"


h_fnc *h_alloc_fnc ( void )
{
  h_fnc * f = (h_fnc *) malloc ( sizeof( h_fnc ) );

  if ( f == NULL )
      _STAT_MSG ( "Allocating h_fnc",
                  "cannot allocate h_fnc",
                  ERROR, 0 );
  else {
      f->C_da = NULL;
      f->deriv = NULL;
      f->fc = NULL;
      f->step_T = NULL;
      f->params = NULL;
      
      return f;
  }
}



int h_init_fnc_cauchy_data ( h_fnc *f, void *params, int rank, ... )
{
  char *fnc_msg = "Initializing Cauchy data in h_fnc structure";
  
  va_list arguments;

  int i;
  
  H_DBL (*fnc_ptr)( H_DBL, void * );

  if ( f == NULL )
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  ERROR, 0 );
  
  f->C_da = (_fnc_1D *) malloc ( rank*sizeof( _fnc_1D ) );

  if ( f->C_da == NULL )
      _STAT_MSG ( fnc_msg,
                  "cannot allocate h_fnc Cauchy data pointer",
                  ERROR, 0 );
  
  va_start ( arguments, rank );
  
  for (i = 0; i < rank; i++) {
      fnc_ptr = va_arg ( arguments, H_DBL (*)(H_DBL, void*) );
      if ( fnc_ptr == NULL ) {
          va_end ( arguments );
          _STAT_MSG ( fnc_msg,
                      "number of arguments not equal to rank",
                      ERROR, 0 );
      }
      else {
          f->C_da[i] = fnc_ptr;
      }
  }

  f->rank = rank;
  
  f->params = (void *) params;

  _STAT_MSG ( fnc_msg,
              NULL,
              OK, 0 );

  return H_TRUE;
}



int h_init_fnc_derivs ( h_fnc *f, int ISN, ... ) /* Independent Stencil Number */
{
  char *fnc_msg = "Initializing RHS derivs in h_fnc structure";
  
  va_list arguments;
  
  int i;
  
  int (*fnc_ptr)(H_DBL, H_DBL*, H_DBL*, H_DBL*, int, int, void*);

  if ( f == NULL )
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  ERROR, 0 );
  
  f->deriv = (_deriv_1D *) malloc ( ISN*sizeof( _deriv_1D ) );

  if ( f->deriv == NULL )
      _STAT_MSG ( fnc_msg,
                  "cannot allocate deriv pointer",
                  ERROR, 0 );

  va_start ( arguments, ISN );
  
  for (i = 0; i < ISN; i++) {
      fnc_ptr = va_arg ( arguments, int (*)(H_DBL, H_DBL*, H_DBL*,
                                            H_DBL*, int, int, void*) );
      if ( fnc_ptr == NULL ) {
          va_end ( arguments );
          _STAT_MSG ( fnc_msg,
                      "number of arguments not equal to ISN",
                      ERROR, 0 );
      }
      else {
          f->deriv[i] = fnc_ptr;
      }
  }

  f->ISN = ISN;
  
  _STAT_MSG ( fnc_msg,
              NULL,
              OK, 0 );

  return H_TRUE;
}



int h_init_fnc_flag_crit ( h_fnc *f, _flag_crit fc )
{
  char *fnc_msg = "Initializing flag criterion in h_fnc structure";

  if ( f == NULL )
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  ERROR, 0 );
  else {
      f->fc = fc; /* TODO: */
      _STAT_MSG ( fnc_msg,
                  NULL,
                  OK, 0 );
  }
  
  return H_TRUE;
}



int h_init_fnc_step_type ( h_fnc *f, const gsl_odeiv_step_type * s )
{
  char *fnc_msg = "Initializing step type in h_fnc structure";

  if ( f == NULL )
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  ERROR, 0 );
  else {
      f->step_T = s;
      _STAT_MSG ( fnc_msg,
                  NULL,
                  OK, 0 );
  }
  
  return H_TRUE;
}


void h_free_fnc ( h_fnc *f )
{
  char *fnc_msg = "Freeing h_fnc structure";

  if ( f != NULL ) {
      if ( f->C_da != NULL ) {
          free ( f->C_da );
      }
      if ( f->deriv != NULL ) {
          free ( f->deriv );
      }
      /* if ( f->fc != NULL ) { */
      /*     free ( f->fc ); */
      /* } */
      /* if ( f->step_T != NULL ) { */
      /*     /\* f->step_T->free(  ); *\/ */
      /*     /\* free ( f->step_T ); *\/ */
      /* } */
      if ( f->params != NULL ) {
          free( f->params );
      }
      
      free( f );
      f = NULL;
  }
  else
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  WARNING, 0 );
}
