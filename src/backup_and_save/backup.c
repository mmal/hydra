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
  bas->file_id = H5Fcreate ( "test.h5",
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




void _h_save_grid_data ( h_grid *grid )
{
  int i;
  hid_t       file_id, dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims[1];
  herr_t      status;
  
  const int rank = grid->rank; 
  const int N = grid->Ntotal;
  
  /* dims = (hsize_t*) malloc( (1)*sizeof(hsize_t) ); */
  
  /* Create a new file using default properties. */
  file_id = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  
  /* Create the data space for the dataset. */
  dims[0] = N*rank;
  
  dataspace_id = H5Screate_simple ( 1, dims, NULL );

  /* Create the dataset. */
  dataset_id = H5Dcreate ( file_id,
                           "/u",
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
  dataset_id = H5Dcreate ( file_id,
                           "/x",
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

  /* Close the file. */
  status = H5Fclose ( file_id );
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
  /* hid_t  *file_id = bas->file_id; */
  /* hid_t *gset_id = &bas->gset_id; */
  /* hid_t *glevel_id = bas->glevel_id; */
  /* hid_t *grid_id = bas->grid_id; */

  
  herr_t      status;
  
  const int l = glevel->l;
  const int M = glevel->M;
  int m;

  /* Create a new group named "grid_l" in the "gset" group */
  bas->glevel_id = H5Gcreate ( bas->gset_id, _h_create_glevel_name( l ), 0 );

  for (m = 0; m < M; m++) {

      /* Create a new group named "grid_l_m" in the "grid_l" group */
      bas->grid_id = H5Gcreate ( bas->glevel_id, _h_create_grid_name( l, m ), 0 );
      
      /* _h_save_grid ( bas, glevel->grid[m] ); */

      /* Close the "grid_l_m" group */
      status = H5Gclose ( bas->grid_id );
  }
  
  /* Close the "glevel_l" group */
  status = H5Gclose ( bas->glevel_id );
  
   
   /* /\* Create a new file using default properties. *\/ */
   /* file_id = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT); */

   /* /\* Create group "MyGroup" in the root group using absolute name. *\/ */
   /* group1_id = H5Gcreate(file_id, "/MyGroup", 0); */

   /* /\* Create group "Group_A" in group "MyGroup" using absolute name. *\/ */
   /* group2_id = H5Gcreate(file_id, "/MyGroup/Group_A", 0); */

   /* /\* Create group "Group_B" in group "MyGroup" using relative name. *\/ */
   /* group3_id = H5Gcreate(group1_id, "Group_B", 0); */

   /* /\* Close groups. *\/ */
   /* status = H5Gclose(group1_id); */
   /* status = H5Gclose(group2_id); */
   /* status = H5Gclose(group3_id); */

   /* /\* Close the file. *\/ */
   /* status = H5Fclose(file_id); */

   /* status = H5Eclear1(); */
   
}

void _h_save_grid ( h_bas *bas, h_glevel *grid )
{
  
}
/* { */
/*   /\* int i; *\/ */
/*   /\* hid_t       file_id, dataset_id, dataspace_id;  /\\* identifiers *\\/ *\/ */
/*   /\* hsize_t     dims; *\/ */
/*   /\* herr_t      status; *\/ */
  
/*   /\* const int rank = grid->rank;  *\/ */
/*   /\* const int N = grid->Ntotal; *\/ */

/*    hid_t       file_id, group_id, dataset_id, dataspace_id;  /\* identifiers *\/ */
/*    hsize_t     dims[2]; */
/*    herr_t      status; */
/*    int         i, j, dset1_data[3][3], dset2_data[2][10]; */

/*    /\* Initialize the first dataset. *\/ */
/*    for (i = 0; i < 3; i++) */
/*       for (j = 0; j < 3; j++) */
/*          dset1_data[i][j] = j + 1; */

/*    /\* Initialize the second dataset. *\/ */
/*    for (i = 0; i < 2; i++) */
/*       for (j = 0; j < 10; j++) */
/*          dset2_data[i][j] = j + 1; */

/*    /\* Open an existing file. *\/ */
/*    /\* file_id = H5Fopen(FILE, H5F_ACC_RDWR, H5P_DEFAULT); *\/ */
/*    file_id = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT); */

/*    /\* Create the data space for the first dataset. *\/ */
/*    dims[0] = 3; */
/*    dims[1] = 3; */
/*    dataspace_id = H5Screate_simple(2, dims, NULL); */

  
/*    /\* Open an existing file. *\/ */
/*    file_id = H5Fopen(FILE, H5F_ACC_RDWR, H5P_DEFAULT); */

/*    /\* Create the data space for the first dataset. *\/ */
/*    dims[0] = 3; */
/*    dims[1] = 3; */
/*    dataspace_id = H5Screate_simple(2, dims, NULL); */

/*    /\* Create a dataset in group "MyGroup". *\/ */
/*    dataset_id = H5Dcreate(file_id, "/MyGroup/dset1", H5T_STD_I32BE, dataspace_id, */
/*                           H5P_DEFAULT); */

/*    /\* Write the first dataset. *\/ */
/*    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, */
/*                      dset1_data); */

/*    /\* Close the data space for the first dataset. *\/ */
/*    status = H5Sclose(dataspace_id); */

/*    /\* Close the first dataset. *\/ */
/*    status = H5Dclose(dataset_id); */

/*    /\* Open an existing group of the specified file. *\/ */
/*    group_id = H5Gopen(file_id, "/MyGroup/Group_A"); */

/*    /\* Create the data space for the second dataset. *\/ */
/*    dims[0] = 2; */
/*    dims[1] = 10; */
/*    dataspace_id = H5Screate_simple(2, dims, NULL); */

/*    /\* Create the second dataset in group "Group_A". *\/ */
/*    dataset_id = H5Dcreate(group_id, "dset2", H5T_STD_I32BE, dataspace_id, H5P_DEFAULT); */

/*    /\* Write the second dataset. *\/ */
/*    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, */
/*                      dset2_data); */

/*    /\* Close the data space for the second dataset. *\/ */
/*    status = H5Sclose(dataspace_id); */

/*    /\* Close the second dataset *\/ */
/*    status = H5Dclose(dataset_id); */

/*    /\* Close the group. *\/ */
/*    status = H5Gclose(group_id); */

/*    /\* Close the file. *\/ */
/*    status = H5Fclose(file_id); */
/* } */

/* void _h_save_grid_stuct ( ); */
    

/* void h_save_grid ( h_grid *grid ) */
