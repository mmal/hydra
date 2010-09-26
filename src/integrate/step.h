#ifndef _STEP_H_
#define _STEP_H_

#include <math.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_errno.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/hms_common.h"



int RHS_jac ( H_DBL t, const H_DBL y[], H_DBL *dfdy, 
              H_DBL dfdt[], void *params );

int RHS_eq ( H_DBL t, const H_DBL y[], H_DBL f[], void *params );

int _h_step ( H_DBL t0, H_DBL t1, H_DBL dt,
              H_DBL *u, h_hms *m );

#endif /* _STEP_H_ */
