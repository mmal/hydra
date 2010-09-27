

#include "hms_common.h"



h_hms *h_alloc_hms ( void )
{
  h_hms * m = (h_hms *) malloc ( sizeof( h_hms ) );

  if ( m == NULL ) {
      _STAT_MSG ( "Allocating hms",
                  "cannot allocate hms",
                  WARNING, 0 );
  }
  else {
      h_gset * g = h_alloc_gset ( );
      h_amrp * p = h_alloc_amrp ( );
      h_fnc * f = h_alloc_fnc ( );

      m->gset = g;
      m->amrp = p;
      m->fnc = f;
  }
  return m;
}


void h_free_hms ( h_hms *m )
{
  if ( m!=NULL ) {
      if ( m->gset!=NULL) {
          h_free_gset ( m->gset );
      }
      
      if ( m->amrp!=NULL) {
          h_free_amrp ( m->amrp );
      }
      
      if ( m->fnc!=NULL) {
          h_free_fnc ( m->fnc );
      }
  }
  
  free ( m );
  
  m = NULL;
}


/* h_hms *h_alloc_hms ( void ) */
/* { */
/*   h_hms * m = (h_hms *) malloc ( sizeof( h_hms ) ); */

/*   if ( m == NULL ) */
/*       _STAT_MSG ( "Allocating hms", */
/*                   "cannot allocate hms", */
/*                   ERROR, 0 ); */

/*   h_grid * g = h_alloc_grid ( ); */
  
/*   h_amrp * p = h_alloc_amrp ( ); */
  
/*   h_fnc * f = h_alloc_fnc ( ); */

/*   m->g = g; */
/*   m->p = p; */
/*   m->f = f; */

/*   return m; */
/* } */


/* void h_free_hms ( h_hms *m ) */
/* { */
/*   if ( m!=NULL ) { */
/*       if ( m->g!=NULL) { */
/*           h_free_grid ( m->g ); */
/*       } */
      
/*       if ( m->p!=NULL) { */
/*           h_free_amrp ( m->p ); */
/*       } */
      
/*       if ( m->f!=NULL) { */
/*           h_free_fnc ( m->f ); */
/*       } */
/*   } */
  
/*   free ( m ); */
  
/*   m = NULL; */
/* } */
