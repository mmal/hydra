/**
 * @file   log.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Tue Nov  2 18:11:42 2010
 * 
 * @brief  
 * 
 * 
 */


#include "log.h"



h_log *h_alloc_log ( void )
{
  h_log *log = (h_log*) malloc ( sizeof( h_log ) );

  if ( log == NULL ) {
      /* _STAT_MSG ( "Allocating h_log", */
      /*             "cannot allocate h_log", */
      /*             WARNING, 0 ); */
  }

  log->h = lwl_alloc ();
  
  return log;
}
