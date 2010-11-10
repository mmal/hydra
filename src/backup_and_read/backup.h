#ifndef _BACKUP_H_
#define _BACKUP_H_


#include <hdf5.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"
/* #include "src/common/utils_common.h" */
/* #include "src/common/hms_common.h" */


#define MAX_NAME_LEN 25
#define GLEVEL_NAME_MASK "glevel_%d"
#define GRID_NAME_MASK "grid_%d_%d"


typedef struct
{

  H_DBL t_last;

  hid_t file_id;
  hid_t gset_id;
  hid_t glevel_id;
  hid_t grid_id;

  hid_t  h_grid_id;
  hid_t  h_amrp_id;

} h_bas;


h_bas *h_alloc_bas ( void );
void h_free_bas ( h_bas *b );

char *_h_create_glevel_name ( int l );
char *_h_create_grid_name ( int l, int m );


/* Create the memory data type for HDF5 lib */
void _h_write_h5_grid_struct_t ( h_bas *bas );
void _h_write_h5_amrp_struct_t ( h_bas *bas );


/* Open/close H5 file */
void h_open_create_h5_file ( const char *fname, h_bas *bas );
void h_open_existing_h5_file ( const char *fname, h_bas *bas );
void h_close_h5_file ( h_bas *bas );


/* Write H5 functions */
void h_write_h5_gset ( h_bas *bas, const char *fname, h_gset *gset );
void _h_write_h5_gset ( h_bas *bas, h_gset *gset );
void _h_write_h5_gset_att ( h_bas *bas, h_gset *gset );
void _h_write_h5_glevel ( h_bas *bas, h_glevel *glevel );
void _h_write_h5_glevel_att ( h_bas *bas, h_glevel *glevel );
void _h_write_h5_grid ( h_bas *bas, h_grid *grid );
void _h_write_h5_grid_data ( h_bas *bas, h_grid *grid );
void _h_write_h5_grid_struct ( h_bas *bas, h_grid *grid );

void h_write_h5_amrp ( h_bas *bas, const char *fname, h_amrp *amrp );
void _h_write_h5_amrp_struct ( h_bas *bas, h_amrp *amrp );


/* Read H5 functions */
void h_read_h5_gset (  h_bas *bas, const char *fname, h_gset *gset );
void _h_read_h5_gset ( h_bas *bas, h_gset *gset );
void _h_read_h5_gset_att ( h_bas *bas, h_gset *gset );
void _h_read_h5_glevel ( h_bas *bas, h_glevel *glevel );
void _h_read_h5_glevel_att ( h_bas *bas, h_glevel *glevel );
void _h_read_h5_grid ( h_bas *bas, h_grid *grid );
void _h_read_h5_grid_struct ( h_bas *bas, h_grid *grid );
void _h_read_h5_grid_data ( h_bas *bas, h_grid *grid );

void h_read_h5_amrp (  h_bas *bas, const char *fname, h_amrp *amrp );
void _h_read_h5_amrp_struct ( h_bas *bas, h_amrp *amrp );


#endif /* _BACKUP_H_ */
