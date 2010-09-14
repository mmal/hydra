

#include "fnc_common.h"


h_fnc *h_alloc_fnc ( void *params, int rank, ... )
{
  va_list arguments;

  int i;
  
  H_DBL (*fnc_ptr)( H_DBL, void * );

  h_fnc * f = (h_fnc *) malloc ( sizeof( h_fnc ) );

  if ( f == NULL )
      _STAT_MSG ( "Allocating fnc",
                  "cannot allocate fnc",
                  ERROR, 0 );
  
  /* f->C_data = (void **) malloc ( rank*sizeof( _fnc_1D ) ); */

  f->C_da = (_fnc_1D *) malloc ( rank*sizeof( _fnc_1D ) );
  if ( f->C_da == NULL )
      _STAT_MSG ( "Allocating fnc",
                  "cannot allocate fnc Cauchy data pointer",
                  ERROR, 0 );
  
  va_start ( arguments, rank );

  for (i = 0; i < rank; i++) {
      fnc_ptr = va_arg ( arguments, H_DBL (*)(H_DBL, void*) );
      if ( fnc_ptr == NULL ) {
          va_end ( arguments );
          _STAT_MSG ( "Allocating fnc",
                      "number of arguments is not equal to rank",
                      ERROR, 0 );
      }
      else {
          f->C_da[i] = fnc_ptr;
      }
  }

  f->rank = rank;
  
  f->params = (void *) params;

  _STAT_MSG ( "Allocating fnc",
              NULL,
              OK, 0 );
  return f;
}


void h_free_fnc ( h_fnc *f )
{
  if ( f != NULL ) {
      if ( f->C_da != NULL ) {
          /* VL(("f->C_da != NULL\n" )); */
          free ( f->C_da );
      }
      
      free( f );

      f = NULL;
      
      _STAT_MSG ( "Feeing fnc",
                  NULL,
                  OK, 0 );
  }
  else
      _STAT_MSG ( "Feeing fnc",
                  "fnc was not allocated",
                  WARNING, 0 );
}
