#ifndef _UPDATE_H_
#define _UPDATE_H_

#include <float.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/hms_common.h"



int _h_update_grid_interior ( h_grid *parent, h_grid *child, h_amrp *amrp );
int _h_update_grid_ghosts ( h_grid *parent, h_grid *child, h_amrp *amrp );

int _h_update_grid_all  ( h_grid *parent, h_grid *child, h_amrp *amrp );

int _h_update_grid_ghosts_left ( h_grid *parent, h_grid *child, h_amrp *amrp );
int _h_update_grid_ghosts_right ( h_grid *parent, h_grid *child, h_amrp *amrp );

int _h_update_grid ( h_grid *parent, h_grid *child, h_amrp *amrp );
int _h_update_glevel ( h_glevel *parent, h_glevel *child, h_amrp *amrp );


#endif /* _UPDATE_H_ */
