#ifndef _H_GENERATE_GRID_H_
#define _H_GENERATE_GRID_H_

#include <math.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"


void h_gen_grid_family ( h_grid * cg, h_amrp *p );

void h_gen_off_grid ( h_grid *cg, h_amrp *p, int l );

#endif /* _H_GENERATE_GRID_H_ */
