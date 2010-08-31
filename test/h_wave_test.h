#ifndef _H_INTEGR_TEST_H_
#define _H_INTEGR_TEST_H_

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include "src/common/h_types.h"
#include "src/h_grids_common.h"
#include "src/error_log/error_printf.h"
#include "src/der_fda/derivs.h"
#include "src/ivp/ivp_1D.h"
#include "src/plot/h_plot_1D.h"
#include "src/h_amrp.h"

#include "src/gener_grid/h_generate_grid.h"


int func_wave (H_DBL t, const H_DBL y[], H_DBL f[],
               void *params);
     
int jac_wave (H_DBL t, const H_DBL y[], H_DBL *dfdy, 
              H_DBL dfdt[], void *params);

void init_v ( int dim, ... );

H_DBL lin ( H_DBL x, void * params );

    
#endif /* _H_INTEGR_TEST_H_ */
