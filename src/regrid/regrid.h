#ifndef _REGRID_H_
#define _REGRID_H_

#include <unistd.h>

#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"

#include "src/create_grid/flagging.h"
#include "src/create_grid/flag_criterion.h"

#include "src/interpolate/interpolate.h"

int h_regrid ( h_hms * hms );
int _h_regrid_glevel ( h_gset *gset, h_amrp *amrp, h_fnc *fnc, int l );

#endif /* _REGRID_H_ */
