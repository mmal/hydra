

#include "amrp_common.h"


h_amrp *h_alloc_amrp ( void )
{
  h_amrp * p = (h_amrp*) malloc ( sizeof( h_amrp ) );

  if ( p == NULL )
      _STAT_MSG ( "Allocating amrp",
                  "cannot allocate amrp",
                  ERROR, 0 );
  else
      _STAT_MSG ( "Allocating amrp",
                  NULL,
                  OK, 0 );
  return p;
}


void h_free_amrp ( h_amrp *p )
{
  if ( p != NULL ) {
      free( p );
      _STAT_MSG ( "Feeing amrp",
                  NULL,
                  OK, 0 );
  }
  
  else
      _STAT_MSG ( "Feeing amrp",
                  "amrp was not allocated",
                  WARNING, 0 );
}
