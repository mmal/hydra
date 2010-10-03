#ifndef _STEP_H_
#define _STEP_H_

#include <math.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_errno.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/hms_common.h"


typedef struct
{
  h_grid *grid;

  h_amrp *amrp;

  h_fnc *fnc;
  
} h_hss;



int RHS_jac ( H_DBL t, const H_DBL y[], H_DBL *dfdy, 
              H_DBL dfdt[], void *params );

int RHS_eq ( H_DBL t, const H_DBL y[], H_DBL f[], void *params );

int _h_boialg_step_glevel ( h_glevel *glevel, h_amrp *amrp, h_fnc *fnc );

int _h_boialg_step_grid ( h_grid *grid, h_amrp *amrp, h_fnc *fnc );

#endif /* _STEP_H_ */
