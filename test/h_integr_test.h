#ifndef _H_INTEGR_TEST_H_
#define _H_INTEGR_TEST_H_

#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/stat_log/stat_msg.h"



int func (double t, const double y[], double f[],
          void *params);
     
int jac (double t, const double y[], double *dfdy, 
         double dfdt[], void *params);

#endif /* _H_INTEGR_TEST_H_ */
