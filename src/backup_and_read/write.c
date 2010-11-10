/**
 * @file   write.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Mon Nov  8 14:24:11 2010
 * 
 * @brief  Write data/backup
 * 
 * 
 */


#include "backup.h"


void h_write_h5_gset ( h_bas *bas, const char *fname, h_gset *gset )
{
  h_open_create_h5_file ( fname, bas );

  _h_write_h5_gset ( bas, gset );

  h_close_h5_file ( bas );
}


void _h_write_h5_gset ( h_bas *bas, h_gset *gset )
{
  herr_t      status;
  const int L = gset->L;
  int l;
  
  /* Create a group named "/gset" in the file */
  bas->gset_id = H5Gcreate( bas->file_id, "/gset", 0);

  /* Write gset attributes */
  _h_write_h5_gset_att ( bas, gset );
  

  /* Write all glevels in gset */ 
  for (l = 0; l < L; l++) {
      _h_write_h5_glevel ( bas, gset->glevel[l] );
  }

  /* Close the "/gset" group */
  status = H5Gclose ( bas->gset_id );
}



void _h_write_h5_gset_att ( h_bas *bas, h_gset *gset )
{
  hsize_t     dim_att;
  hid_t       dataspace_id_att, attribute_id;
  int         att_data[1];
  herr_t      status;
  
  /* Initialize the attribute data. */
  att_data[0] = gset->L;

  /* Create the data space for the attribute. */
  dim_att = 1;
  dataspace_id_att = H5Screate_simple( 1, &dim_att, NULL ); 
  
  /* Create a dataset attribute. */
  attribute_id = H5Acreate ( bas->gset_id,
                             "attr",
                             H5T_STD_I32BE,
                             dataspace_id_att,
                             H5P_DEFAULT );

  /* Write the attribute data. */
  status = H5Awrite ( attribute_id,
                      H5T_NATIVE_INT,
                      att_data );

  /* Close the attribute. */
  status = H5Aclose ( attribute_id );

  /* Close the dataspace. */
  status = H5Sclose ( dataspace_id_att );
}



void _h_write_h5_glevel ( h_bas *bas, h_glevel *glevel )
{
  herr_t      status;
  const int l = glevel->l;
  const int M = glevel->M;
  int m;

  /* Create a new group named "glevel_l" in the "gset" group */
  bas->glevel_id = H5Gcreate ( bas->gset_id, _h_create_glevel_name ( l ), 0 );

  /* Write glevel attributes */
  _h_write_h5_glevel_att ( bas, glevel );
  
  for (m = 0; m < M; m++) {

      /* Create a new group named "grid_l_m" in the "grid_l" group */
      bas->grid_id = H5Gcreate ( bas->glevel_id,
                                 _h_create_grid_name ( l, m ), 0 );
      
      _h_write_h5_grid ( bas, glevel->grid[m] );

      /* Close the "grid_l_m" group */
      status = H5Gclose ( bas->grid_id );
  }
  
  /* Close the "glevel_l" group */
  status = H5Gclose ( bas->glevel_id );
}



void _h_write_h5_glevel_att ( h_bas *bas, h_glevel *glevel )
{
  hsize_t     dim_att;
  hid_t       dataspace_id_att, attribute_id;
  int         att_data[2];
  herr_t      status;
  
  /* Initialize the attribute data. */
  att_data[0] = glevel->M;
  att_data[1] = glevel->l;

  /* Create the data space for the attribute. */
  dim_att = 2;
  dataspace_id_att = H5Screate_simple( 1, &dim_att, NULL ); 
  
  /* Create a dataset attribute. */
  attribute_id = H5Acreate ( bas->glevel_id,
                             "attr",
                             H5T_STD_I32BE,
                             dataspace_id_att,
                             H5P_DEFAULT );

  /* Write the attribute data. */
  status = H5Awrite ( attribute_id,
                      H5T_NATIVE_INT,
                      att_data );

  /* Close the attribute. */
  status = H5Aclose ( attribute_id );

  /* Close the dataspace. */
  status = H5Sclose ( dataspace_id_att );
}



void _h_write_h5_grid ( h_bas *bas, h_grid *grid )
{
  /* Write grid positions and data */
  _h_write_h5_grid_data ( bas, grid );

  /* Write grid parameter stored in h_grid structure */
  _h_write_h5_grid_struct ( bas, grid );
}



void _h_write_h5_grid_data ( h_bas *bas, h_grid *grid )
{
  hid_t       dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims;
  herr_t      status;
  
  const int rank = grid->rank; 
  const int N = grid->Ntotal;
  
  
  /* Create the data space for the dataset. */
  dims = N*rank;
  
  dataspace_id = H5Screate_simple ( 1, &dims, NULL );

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


  dims = N;
  dataspace_id = H5Screate_simple( 1, &dims, NULL );
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



void _h_write_h5_grid_struct ( h_bas *bas, h_grid *grid )
{
  hid_t       dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims;
  herr_t      status;
  
  /* Create the data space for the dataset. */
  dims = 1;
  dataspace_id = H5Screate_simple ( 1, &dims, NULL );

  /* Create the dataset. */
  dataset_id = H5Dcreate ( bas->grid_id,
                           "struct",
                           bas->h_grid_id,
                           dataspace_id,
                           H5P_DEFAULT );
  
  /* Write the dataset */
  status = H5Dwrite ( dataset_id,
                      bas->h_grid_id,
                      H5S_ALL,
                      H5S_ALL,
                      H5P_DEFAULT,
                      grid );

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );


  /* Terminate access to the data space. */ 
  status = H5Sclose ( dataspace_id );
}


void h_write_h5_amrp ( h_bas *bas, const char *fname, h_amrp *amrp )
{
  h_open_create_h5_file ( fname, bas );

  _h_write_h5_amrp_struct ( bas, amrp );

  h_close_h5_file ( bas );
}


void _h_write_h5_amrp_struct ( h_bas *bas, h_amrp *amrp )
{
  hid_t       dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims;
  herr_t      status;
  

  /* Create the data space for the dataset. */
  dims = 1;
  dataspace_id = H5Screate_simple ( 1, &dims, NULL );

  /* Create the dataset. */
  dataset_id = H5Dcreate ( bas->file_id,
                           "amrp",
                           bas->h_amrp_id,
                           dataspace_id,
                           H5P_DEFAULT );
  
  /* Write the dataset */
  status = H5Dwrite ( dataset_id,
                      bas->h_amrp_id,
                      H5S_ALL,
                      H5S_ALL,
                      H5P_DEFAULT,
                      amrp );

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose ( dataset_id );


  /* Terminate access to the data space. */ 
  status = H5Sclose ( dataspace_id );
}



void _h_write_h5_grid_struct_t ( h_bas *bas )
{
  /* Memory datatype handler */
  bas->h_grid_id = H5Tcreate ( H5T_COMPOUND, sizeof(h_grid) );

  /* Create the memory data type */
  H5Tinsert( bas->h_grid_id, "N", HOFFSET(h_grid, N), H5T_NATIVE_INT );
  H5Tinsert( bas->h_grid_id, "rank", HOFFSET(h_grid, rank), H5T_NATIVE_INT );
  H5Tinsert( bas->h_grid_id, "l", HOFFSET(h_grid, l), H5T_NATIVE_INT );
  H5Tinsert( bas->h_grid_id, "m", HOFFSET(h_grid, m), H5T_NATIVE_INT );
  H5Tinsert( bas->h_grid_id, "xL", HOFFSET(h_grid, xL), H5T_NATIVE_DOUBLE );
  H5Tinsert( bas->h_grid_id, "xR", HOFFSET(h_grid, xR), H5T_NATIVE_DOUBLE );
  H5Tinsert( bas->h_grid_id, "h", HOFFSET(h_grid, h), H5T_NATIVE_DOUBLE );
  H5Tinsert( bas->h_grid_id, "dt", HOFFSET(h_grid, dt), H5T_NATIVE_DOUBLE );
  H5Tinsert( bas->h_grid_id, "is_master", HOFFSET(h_grid, is_master), H5T_NATIVE_INT );
  
  H5Tinsert( bas->h_grid_id, "Nchildren", HOFFSET(h_grid, Nchildren), H5T_NATIVE_INT );

  H5Tinsert( bas->h_grid_id, "t", HOFFSET(h_grid, t), H5T_NATIVE_DOUBLE );
  H5Tinsert( bas->h_grid_id, "tlast", HOFFSET(h_grid, tlast), H5T_NATIVE_DOUBLE );
  H5Tinsert( bas->h_grid_id, "Ncalls", HOFFSET(h_grid, Ncalls), H5T_NATIVE_INT );

  H5Tinsert( bas->h_grid_id, "Lghost", HOFFSET(h_grid, Lghost), H5T_NATIVE_INT );
  H5Tinsert( bas->h_grid_id, "Rghost", HOFFSET(h_grid, Rghost), H5T_NATIVE_INT );
  H5Tinsert( bas->h_grid_id, "Ntotal", HOFFSET(h_grid, Ntotal), H5T_NATIVE_INT );

  /* TODO:
   * add other h_grid elements */

}



void _h_write_h5_amrp_struct_t ( h_bas *bas )
{
  /* Memory datatype handler */
  bas->h_amrp_id = H5Tcreate ( H5T_COMPOUND, sizeof(h_amrp) );

  /* Create the memory data type */
  H5Tinsert( bas->h_amrp_id, "rr",   HOFFSET(h_amrp, rr), H5T_NATIVE_INT );
  H5Tinsert( bas->h_amrp_id, "buf",  HOFFSET(h_amrp, buf), H5T_NATIVE_INT );
  H5Tinsert( bas->h_amrp_id, "sp",   HOFFSET(h_amrp, sp), H5T_NATIVE_INT );
  H5Tinsert( bas->h_amrp_id, "lmax", HOFFSET(h_amrp, lmax), H5T_NATIVE_INT );
  H5Tinsert( bas->h_amrp_id, "lmbd", HOFFSET(h_amrp, lmbd), H5T_NATIVE_INT );
  H5Tinsert( bas->h_amrp_id, "ngh",  HOFFSET(h_amrp, ngh), H5T_NATIVE_INT );
  H5Tinsert( bas->h_amrp_id, "gme",  HOFFSET(h_amrp, gme), H5T_NATIVE_INT );
  
  H5Tinsert( bas->h_amrp_id, "errt", HOFFSET(h_amrp, errt), H5T_NATIVE_DOUBLE );

  /* TODO:
   * add other h_amrp elements */
}
