#ifndef _CREATE_TEST_H_
#define _CREATE_TEST_H_

#include <stdio.h>
#include <stdarg.h>
#include <math.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"
#include "src/common/hms_common.h"
#include "src/stat_log/stat_msg.h"


#include "src/derivs_fda/derivs.h"
#include "src/ivp/ivp_1D.h"

#include "src/plot_data/plot_1D.h"

#include "src/create_grid/flagging.h"
#include "src/create_grid/flag_criterion.h"
#include "src/create_grid/create_grid.h"



H_DBL zero_ ( H_DBL x, void *params );
H_DBL one_ ( H_DBL x, void *params );
H_DBL sin_ ( H_DBL x, void *params );

int RHS_centered ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams );

int RHS_extern_0 ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams );

int RHS_extern_1 ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams );

#endif /* _CREATE_TEST_H_ */
