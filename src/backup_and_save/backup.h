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


} h_bas;


void _h_save_grid_data ( h_grid *grid );
void _h_save_glevel ( h_glevel *glevel );
void _h_save_gset ( h_gset *gset );

#endif /* _BACKUP_H_ */
