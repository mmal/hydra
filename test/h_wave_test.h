#ifndef _H_INTEGR_TEST_H_
#define _H_INTEGR_TEST_H_

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/stat_log/stat_msg.h"


#include "src/derivs_fda/derivs.h"
#include "src/ivp/ivp_1D.h"

#include "src/plot_data/plot_1D.h"

#include "src/create_grid/create_grid.h"


int func_wave (H_DBL t, const H_DBL y[], H_DBL f[],
               void *params);
     
int jac_wave (H_DBL t, const H_DBL y[], H_DBL *dfdy, 
              H_DBL dfdt[], void *params);

void init_v ( int dim, ... );

H_DBL lin ( H_DBL x, void * params );

    
#endif /* _H_INTEGR_TEST_H_ */
