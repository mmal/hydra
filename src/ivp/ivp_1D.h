#ifndef _IVP_1D_H_
#define _IVP_1D_H_


#include <stdarg.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/stat_log/stat_msg.h"


void h_ivp_1D ( h_grid *g, void *params, int rank, ... );

void h_ivp_1D_grid_family ( h_grid *cg, h_amrp *p, void *params, int rank, ... );


#endif /* _IVP_1D_H_ */
