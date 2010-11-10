/**
 * @file   backup.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Tue Nov  2 18:11:04 2010
 * 
 * @brief  
 * 
 * 
 */


#include "backup.h"



h_bas *h_alloc_bas ( void )
{
  h_bas *b = (h_bas *) malloc ( sizeof( h_bas ) );

  if ( b == NULL ) {
      _STAT_MSG ( "Allocating h_bas",
                  "cannot allocate h_bas",
                  WARNING, 0 );
  }
  /* TODO:
   * create new fuction which initializes h_bas structure */
  _h_write_h5_grid_struct_t ( b );  
  _h_write_h5_amrp_struct_t ( b );
  
  return b;
}



void h_free_bas ( h_bas *b )
{
  if ( b != NULL ) {
      free ( b );
      b = NULL;
  }
}



char *_h_create_glevel_name ( int l )
{
  char *name = (char*) malloc( MAX_NAME_LEN*sizeof(char) );
  
  if ( sprintf( name, GLEVEL_NAME_MASK, l ) < 0  )
      _STAT_MSG ( "MAX_NAME_LEN exceed",
                  NULL,
                  ERROR, 0 );
  return name;
}



char *_h_create_grid_name ( int l, int m )
{
  char *name = (char*) malloc( MAX_NAME_LEN*sizeof(char) );
  
  if ( sprintf( name, GRID_NAME_MASK, l, m ) < 0  )
      _STAT_MSG ( "MAX_NAME_LEN exceed",
                  NULL,
                  ERROR, 0 );
  return name;
}



void h_open_create_h5_file ( const char *fname, h_bas *bas )
{
  /* Create a new file using default properties. */
  bas->file_id = H5Fcreate ( fname,
                             H5F_ACC_TRUNC,
                             H5P_DEFAULT,
                             H5P_DEFAULT );
}



void h_open_existing_h5_file ( const char *fname, h_bas *bas )
{
  /* Open an existing file using default properties. */
  bas->file_id = H5Fopen ( fname,
                           H5F_ACC_RDWR,
                           H5P_DEFAULT );
}



void h_close_h5_file ( h_bas *bas )
{
  herr_t status;

  /* Close the file. */
  status = H5Fclose ( bas->file_id );
}
