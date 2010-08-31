#ifndef _IVP_1D_H_
#define _IVP_1D_H_

#include <stdarg.h>

#include "src/h_grids_common.h"

#include "src/error_log/error_printf.h"

#include "src/h_amrp.h"

void h_ivp_1D ( h_grid *g, void *params, int ord, ... );

void h_ivp_1D_grid_family ( h_grid *cg, h_amrp *p, void *params, int ord, ... );

#endif /* _IVP_1D_H_ */
