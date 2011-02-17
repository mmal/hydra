/**
 * @file   pos_common.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Sat Feb 12 16:27:57 2011
 * 
 * @brief  The h_pos structures management functions.
 * 
 * 
 */


#include "pos_common.h"



/** 
 * Allocates memory for a h_pos structure.
 * 
 * @return pointer to the h_pos structure
 */
h_pos * h_alloc_pos ( void )
{
  h_pos * p = (h_pos*) malloc ( sizeof( h_pos ) );

  if ( p == NULL )
      _STAT_MSG ( "Allocating h_pos",
                  "cannot allocate h_pos",
                  WARNING, 0 );
  return p;
}


/** 
 * Frees the memory space pointed by p
 * 
 * @param p pointer to the h_pos structure
 */
void h_free_pos ( h_pos *p )
{
  char *fnc_msg = "Freeing h_pos structure";

  if ( p != NULL ) {
      free( p );
      p = NULL;
  }
  else
      _STAT_MSG ( fnc_msg,
                  "h_pos is unallocated",
                  WARNING, 0 );
}
