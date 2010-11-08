#ifndef _BACKUP_H_
#define _BACKUP_H_


#include <hdf5.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"
/* #include "src/common/utils_common.h" */
/* #include "src/common/hms_common.h" */


#define FILE "dset.h5"

#define TEST_FILE "test.h5"

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
void _h_open_file ( h_bas *bas );
void _h_close_file ( h_bas *bas );

void _h_save_gset ( h_bas *bas, h_gset *gset );
void _h_save_glevel ( h_bas *bas, h_glevel *glevel );
void _h_save_grid ( h_bas *bas, h_grid *grid );

void _h_save_grid_data ( h_bas *bas, h_grid *grid );
void _h_save_grid_struct ( h_bas *bas, h_grid *grid );

void _h_save_grid_struct_t ( h_bas *bas );

void _h_save_amrp_struct_t ( h_bas *bas );


#endif /* _BACKUP_H_ */
