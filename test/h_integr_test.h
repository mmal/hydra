#ifndef _H_INTEGR_TEST_H_
#define _H_INTEGR_TEST_H_

#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>


#include "src/common/h_types.h"
#include "src/h_grids_common.h"
#include "src/error_log/error_printf.h"



int func (double t, const double y[], double f[],
          void *params);
     
int jac (double t, const double y[], double *dfdy, 
         double dfdt[], void *params);

#endif /* _H_INTEGR_TEST_H_ */
