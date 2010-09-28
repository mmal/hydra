#ifndef _H_GENERATE_GRID_H_
#define _H_GENERATE_GRID_H_

#include <stdarg.h>
#include <math.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"

#include "src/create_grid/flagging.h"
#include "src/create_grid/flag_criterion.h"


void _h_create_child_grid ( h_grid *parent, h_grid **child,
                            h_amrp *p, int m, int idL, int idR );

void _h_create_offspring_grids ( h_grid *cg, h_amrp *p,
                                 int *idL, int *idR, int Ngrids );

void _h_create_set_of_grids ( h_hms *m );

void _h_acd_to_one_grid ( h_grid *g, h_fnc *f );

void _h_assign_cauchy_data ( h_grid *g, h_amrp *p, h_fnc *f );

void h_gen_grid_family ( h_grid * cg, h_amrp *p );

void h_gen_off_grid ( h_grid *cg, h_amrp *p, int l );


#endif /* _H_GENERATE_GRID_H_ */
