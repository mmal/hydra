/**
 * @file   utils_common.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Tue Nov  2 14:17:46 2010
 * 
 * @brief  The h_utils structure management functions.
 * 
 * 
 */


#include "utils_common.h"



/**
 * Allocates memory for a h_utils structure.
 *
 * @return pointer to the h_utils structure
 */
h_utils *h_alloc_utils ( void )
{
  h_utils * u = (h_utils*) malloc ( sizeof( h_utils ) );

  if ( u == NULL )
      _STAT_MSG ( "Allocating h_utils",
                  "cannot allocate h_utils",
                  WARNING, 0 );
  else {
      /* u->bas = h_alloc_bas ( ); */
      /* u->log = h_alloc_log(); */ /* TODO: logger structure */
  }
  

  return u;
}



void h_init_utils ( h_utils *u )
{
  char *fnc_msg = "Initializing h_utils structure";

  FILE  *file = fopen("file", "w");
  FILE  *fileINFO = fopen("info", "w");

  
  lwl_set_attributes ( u->log->h, LWL_TAG_PREFIX,
                       "my example app",
                       LWL_TAG_FILE, file,
                       LWL_TAG_OPTIONS, LWL_OPT_PRIORITY |
                       LWL_OPT_DATE | LWL_OPT_TIME | LWL_OPT_PREFIX |
                       LWL_OPT_PERROR | LWL_OPT_USE_LOCALE , LWL_TAG_DONE );

  lwl_set_attributes ( u->log->h, LWL_TAG_FILE_INFO, fileINFO );
  
  lwl_write_log ( u->log->h, LWL_PRI_INFO, "info test");
  
  lwl_write_log ( u->log->h, LWL_PRI_ALERT, "alert test");
  
  _STAT_MSG ( fnc_msg,
              NULL,
              OK, 0 );

}



/**
 * Frees the memory space pointed by u
 *
 * @param u pointer to the h_utils structure
 */
void h_free_utils ( h_utils *u )
{
  char *fnc_msg = "Freeing h_utils structure";

  if ( u != NULL ) {
      
      if ( u->log != NULL ) {
          /* fclose ( lwl_get_log_file( u->log_hand ) ); */
          /* lwl_free ( u->log->h ); */
      }
      if ( u->bas != NULL ) {
          /* h_free_bas ( u->bas ); */
      }
      free( u );
      u = NULL;
  }
  else
      _STAT_MSG ( fnc_msg,
                  "utils is unallocated",
                  WARNING, 0 );
}

