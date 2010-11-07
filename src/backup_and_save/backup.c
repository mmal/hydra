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
#define FILE "dset.h5"

#define TEST_FILE "test.h5"

#define MAX_NAME_LEN 25
#define GLEVEL_NAME_MASK "glevel_%d"
#define GRID_NAME_MASK "grid_%d_%d"


h_bas *h_alloc_bas ( void )
{
  h_bas *b = (h_bas *) malloc ( sizeof( h_bas ) );

  if ( b == NULL ) {
      _STAT_MSG ( "Allocating h_bas",
                  "cannot allocate h_bas",
                  WARNING, 0 );
  }

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


void _h_open_file ( h_bas *bas )
{
  /* Create a new file using default properties. */
  bas->file_id = H5Fcreate ( TEST_FILE,
                             H5F_ACC_TRUNC,
                             H5P_DEFAULT,
                             H5P_DEFAULT );
}

void _h_close_file ( h_bas *bas )
{
  herr_t status;

  /* Close the file. */
  status = H5Fclose ( bas->file_id );

}



void _h_save_grid_data ( h_bas *bas, h_grid *grid )
{
  hid_t       dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims[1];
  herr_t      status;
  
  const int rank = grid->rank; 
  const int N = grid->Ntotal;
  
  
  /* Create the data space for the dataset. */
  dims[0] = N*rank;
  
  dataspace_id = H5Screate_simple ( 1, dims, NULL );

  /* Create the dataset. */
  dataset_id = H5Dcreate ( bas->grid_id,
                           "u",
                           H5T_IEEE_F64LE,
                           dataspace_id,
                           H5P_DEFAULT );

  /* Write the dataset */
  status = H5Dwrite ( dataset_id,
                      H5T_IEEE_F64LE,
                      H5S_ALL,
                      H5S_ALL,
                      H5P_DEFAULT,
                      grid->u );

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );


  dims[0] = N;
  dataspace_id = H5Screate_simple( 1, dims, NULL );
  /* Create the dataset. */
  dataset_id = H5Dcreate ( bas->grid_id,
                           "x",
                           H5T_IEEE_F64LE,
                           dataspace_id,
                           H5P_DEFAULT );

  /* Write the dataset */
  status = H5Dwrite ( dataset_id,
                      H5T_IEEE_F64LE,
                      H5S_ALL,
                      H5S_ALL,
                      H5P_DEFAULT, 
                      grid->x );

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );

   
  /* Terminate access to the data space. */ 
  status = H5Sclose ( dataspace_id );
}



void _h_save_gset ( h_bas *bas, h_gset *gset )
{
  herr_t      status;

  const int L = gset->L;
  int l;

  _h_open_file ( bas );

  /* Create a group named "/gset" in the file */
  bas->gset_id = H5Gcreate( bas->file_id, "/gset", 0);

  for (l = 0; l < L; l++) {
      _h_save_glevel ( bas, gset->glevel[l] );
  }

  /* Close the "/gset" group */
  status = H5Gclose ( bas->gset_id );

  _h_close_file ( bas );
  
}
  


void _h_save_glevel ( h_bas *bas, h_glevel *glevel )
{
  herr_t      status;
  
  const int l = glevel->l;
  const int M = glevel->M;
  int m;

  /* Create a new group named "grid_l" in the "gset" group */
  bas->glevel_id = H5Gcreate ( bas->gset_id, _h_create_glevel_name( l ), 0 );

  for (m = 0; m < M; m++) {

      /* Create a new group named "grid_l_m" in the "grid_l" group */
      bas->grid_id = H5Gcreate ( bas->glevel_id, _h_create_grid_name( l, m ), 0 );
      
      _h_save_grid ( bas, glevel->grid[m] );

      /* Close the "grid_l_m" group */
      status = H5Gclose ( bas->grid_id );
  }
  
  /* Close the "glevel_l" group */
  status = H5Gclose ( bas->glevel_id );
}



void _h_save_grid ( h_bas *bas, h_grid *grid )
{
  _h_save_grid_data ( bas, grid );
  _h_save_grid_struct ( bas, grid );
}



void _h_save_grid_struct ( h_bas *bas, h_grid *grid )
{
  hid_t       h_grid_id; /* memory datatype handle */
  hid_t       dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims[1];
  herr_t      status;
  

  /* Create the memory data type */
  h_grid_id = H5Tcreate ( H5T_COMPOUND, sizeof(h_grid) );

  H5Tinsert( h_grid_id, "N", HOFFSET(h_grid, N), H5T_NATIVE_INT );
  H5Tinsert( h_grid_id, "rank", HOFFSET(h_grid, rank), H5T_NATIVE_INT );
  H5Tinsert( h_grid_id, "l", HOFFSET(h_grid, l), H5T_NATIVE_INT );
  H5Tinsert( h_grid_id, "m", HOFFSET(h_grid, m), H5T_NATIVE_INT );
  H5Tinsert( h_grid_id, "xL", HOFFSET(h_grid, xL), H5T_NATIVE_DOUBLE );
  H5Tinsert( h_grid_id, "xR", HOFFSET(h_grid, xR), H5T_NATIVE_DOUBLE );
  H5Tinsert( h_grid_id, "h", HOFFSET(h_grid, h), H5T_NATIVE_DOUBLE );
  H5Tinsert( h_grid_id, "dt", HOFFSET(h_grid, dt), H5T_NATIVE_DOUBLE );
  H5Tinsert( h_grid_id, "is_master", HOFFSET(h_grid, is_master), H5T_NATIVE_INT );
  
  H5Tinsert( h_grid_id, "Nchildren", HOFFSET(h_grid, Nchildren), H5T_NATIVE_INT );

  H5Tinsert( h_grid_id, "t", HOFFSET(h_grid, t), H5T_NATIVE_DOUBLE );
  H5Tinsert( h_grid_id, "tlast", HOFFSET(h_grid, tlast), H5T_NATIVE_DOUBLE );
  H5Tinsert( h_grid_id, "Ncalls", HOFFSET(h_grid, Ncalls), H5T_NATIVE_INT );

  H5Tinsert( h_grid_id, "Lghost", HOFFSET(h_grid, Lghost), H5T_NATIVE_INT );
  H5Tinsert( h_grid_id, "Rghost", HOFFSET(h_grid, Rghost), H5T_NATIVE_INT );
  H5Tinsert( h_grid_id, "Ntotal", HOFFSET(h_grid, Ntotal), H5T_NATIVE_INT );

  
  /* Create the data space for the dataset. */
  dims[0] = 1;
  
  dataspace_id = H5Screate_simple ( 1, dims, NULL );

  /* Create the dataset. */
  dataset_id = H5Dcreate ( bas->grid_id,
                           "struct",
                           h_grid_id,
                           dataspace_id,
                           H5P_DEFAULT );
  
  /* Write the dataset */
  status = H5Dwrite ( dataset_id,
                      h_grid_id,
                      H5S_ALL,
                      H5S_ALL,
                      H5P_DEFAULT,
                      grid );

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );


  /* Terminate access to the data space. */ 
  status = H5Sclose ( dataspace_id );

}
