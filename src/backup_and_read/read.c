/**
 * @file   read.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Mon Nov  8 14:26:06 2010
 * 
 * @brief  Read data/backup
 * 
 * 
 */


#include "backup.h"



void h_read_h5_gset (  h_bas *bas, const char *fname, h_gset *gset )
{
  h_open_existing_h5_file ( fname, bas );

  _h_read_h5_gset ( bas, gset );

  h_close_h5_file ( bas );
}


void _h_read_h5_gset ( h_bas *bas, h_gset *gset )
{
  herr_t      status;
  const int L = gset->L;
  int l;

  /* Open a group named "/gset" in the file */
  bas->gset_id = H5Gopen( bas->file_id, "/gset");

  /* Read gset attributes */
  _h_read_h5_gset_att ( bas, gset );

  /* Read a;; glevels in gset */
  for (l = 0; l < L; l++) {

      /* Allocate glevel without grids */
      h_alloc_add_glevel ( gset, l, 0 );

      /* Read glevel */
      _h_read_h5_glevel ( bas, gset->glevel[l] );
  }

  /* Close the "/gset" group */
  status = H5Gclose ( bas->gset_id );
}



void _h_read_h5_gset_att ( h_bas *bas, h_gset *gset )
{
  hid_t       attribute_id;
  int         att_data[1];
  herr_t      status;
  

  /* Open a dataset attribute. */
  attribute_id = H5Aopen_name ( bas->gset_id, "attr" );

  /* Read the attribute data. */
  status  = H5Aread ( attribute_id, H5T_NATIVE_INT, att_data ); 

  /* Initialize the attribute data. */
  gset->L = att_data[0];

  /* Close the attribute. */
  status = H5Aclose ( attribute_id );
}



void _h_read_h5_glevel ( h_bas *bas, h_glevel *glevel )
{
  herr_t      status;
  const int l = glevel->l;
  int M;
  int m;

  /* Open a group named "glevel_l" in the "gset" group */
  bas->glevel_id = H5Gopen ( bas->gset_id, _h_create_glevel_name ( l ) );

  /* Read glevel attributes */
  _h_read_h5_glevel_att ( bas, glevel );

  M = glevel->M;

  glevel->M = 0;

  h_alloc_add_N_grids_to_glevel ( glevel, M );
  
  glevel->M = M;

  for (m = 0; m < M; m++) {

      /* Open a group named "grid_l_m" in the "grid_l" group */
      bas->grid_id = H5Gopen ( bas->glevel_id,
                               _h_create_grid_name ( l, m ) );

      /* Allocate memory for grid */
      glevel->grid[m] = h_alloc_grid ( );
      
      _h_read_h5_grid ( bas, glevel->grid[m] );

      /* Close the "grid_l_m" group */
      status = H5Gclose ( bas->grid_id );
  }
  
  /* Close the "glevel_l" group */
  status = H5Gclose ( bas->glevel_id );
}



void _h_read_h5_glevel_att ( h_bas *bas, h_glevel *glevel )
{
  hid_t       attribute_id;
  int         att_data[2];
  herr_t      status;
  
  
  /* Open a dataset attribute. */
  attribute_id = H5Aopen_name ( bas->glevel_id, "attr" );


  /* Read the attribute data. */
  status = H5Aread ( attribute_id, H5T_NATIVE_INT, att_data ); 


  /* Initialize the attribute data. */
  glevel->M = att_data[0];
  glevel->l = att_data[1];

  /* Close the attribute. */
  status = H5Aclose ( attribute_id );
}



void _h_read_h5_grid ( h_bas *bas, h_grid *grid )
{
  /* Write grid parameter stored in h_grid structure */
  _h_read_h5_grid_struct ( bas, grid );

  /* Read grid positions and data */
  _h_read_h5_grid_data ( bas, grid );
}



void _h_read_h5_grid_struct ( h_bas *bas, h_grid *grid )
{
  hid_t       dataset_id;  /* identifiers */
  herr_t      status;
  

  /* Open the dataset. */
  dataset_id = H5Dopen ( bas->grid_id,
                         "struct" );
  
  /* Read the dataset */
  status = H5Dread ( dataset_id,
                     bas->h_grid_id,
                     H5S_ALL,
                     H5S_ALL,
                     H5P_DEFAULT,
                     grid );

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );

}



void _h_read_h5_grid_data ( h_bas *bas, h_grid *grid )
{
  hid_t       dataset_id;  /* identifiers */
  herr_t      status;
  
  const int rank = grid->rank; 
  const int N = grid->Ntotal;


  grid->x = ( H_DBL *) malloc ( N*sizeof( H_DBL ) );
  grid->u = ( H_DBL *) malloc ( rank*N*sizeof( H_DBL ) );

  
  /* Open the dataset. */
  dataset_id = H5Dopen ( bas->grid_id,
                         "u" );

  /* Read the dataset */
  status = H5Dread ( dataset_id,
                     H5T_IEEE_F64LE,
                     H5S_ALL,
                     H5S_ALL,
                     H5P_DEFAULT,
                     grid->u );


  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );



  /* Open the dataset. */
  dataset_id = H5Dopen ( bas->grid_id,
                         "x" );

  /* Read the dataset */
  status = H5Dread ( dataset_id,
                     H5T_IEEE_F64LE,
                     H5S_ALL,
                     H5S_ALL,
                     H5P_DEFAULT,
                     grid->x );


  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );

}



void h_read_h5_amrp (  h_bas *bas, const char *fname, h_amrp *amrp )
{
  h_open_existing_h5_file ( fname, bas );
  
  _h_read_h5_amrp_struct ( bas, amrp );

  h_close_h5_file ( bas );
}



void _h_read_h5_amrp_struct ( h_bas *bas, h_amrp *amrp )
{
  hid_t       dataset_id;  /* identifiers */
  herr_t      status;
  

  /* Open the dataset. */
  dataset_id = H5Dopen ( bas->file_id,
                         "amrp" );
  
  /* Read the dataset */
  status = H5Dread ( dataset_id,
                     bas->h_amrp_id,
                     H5S_ALL,
                     H5S_ALL,
                     H5P_DEFAULT,
                     amrp );

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );
}
