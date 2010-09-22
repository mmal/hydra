#ifndef _FLAG_CRITERION_H_
#define _FLAG_CRITERION_H_

#include <unistd.h>
#include <math.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_errno.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/hms_common.h"

#include "src/create_grid/create_grid.h"


int _h_fc_integrate ( H_DBL t0, H_DBL t1, H_DBL dt, H_DBL *u, h_hms *m );

int h_fc_Test ( void *vm, H_DBL * tau );

int h_fc_Richardson ( void *vm, H_DBL * tau );



int h_fc_Energy ( void *vm, H_DBL * tau );

int h_fc_SV ( void *vm, H_DBL * tau );

int h_fc_TV ( void *vm, H_DBL * tau );

  
#endif /* _FLAG_CRITERION_H_ */
