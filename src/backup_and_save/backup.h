#ifndef _BACKUP_H_
#define _BACKUP_H_


#include <hdf5.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"
/* #include "src/common/utils_common.h" */
/* #include "src/common/hms_common.h" */


typedef struct
{

  H_DBL t_last;

  hid_t file_id;
  hid_t gset_id;
  hid_t glevel_id;
  hid_t grid_id;

  /* hid_t group_id; */
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
    /* void _h_save_glevel ( h_glevel *glevel ); */
/* void _h_save_gset ( h_gset *gset ); */

#endif /* _BACKUP_H_ */
