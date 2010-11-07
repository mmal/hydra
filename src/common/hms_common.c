/**
 * @file   hms_common.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Wed Nov  3 22:46:03 2010
 * 
 * @brief  The h_hms structure management functions.
 * 
 * 
 */


#include "hms_common.h"



/** 
 * Allocates memory a the h_hms structure
 * and initializes loggger.
 * 
 * 
 * @return pointer to the h_hms structure
 */
h_hms *h_alloc_hms ( void )
{
  h_hms * m;

  /* Initializing main logger */ 
  h_init_log ();

  /* Allocating the h_hms */
  m = (h_hms *) malloc ( sizeof( h_hms ) );

  if ( m == NULL ) {
      _STAT_MSG ( "Allocating h_hms",
                  "cannot allocate h_hms",
                  WARNING, 0 );
  }
  else {
      /* Allocating all components of the h_hms */
      h_gset * g = h_alloc_gset ( );
      h_amrp * p = h_alloc_amrp ( );
      h_fnc * f = h_alloc_fnc ( );
      h_utils * u = h_alloc_utils ( );
      
      
      m->gset = g;
      m->amrp = p;
      m->fnc = f;
      m->utils = u;

  }
  return m;
}



/** 
 * Frees the memory space pointed by m
 * and frees the logger.
 * 
 * @param m pointer to the h_hms structure
 */
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

      if ( m->utils!=NULL) {
          h_free_utils ( m->utils );
      }
      
      free ( m );
      m = NULL;

      h_free_log ( );

  }
}
