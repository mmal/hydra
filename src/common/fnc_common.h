#ifndef _FNC_COMMON_H_
#define _FNC_COMMON_H_


#include <stdarg.h>
#include <gsl/gsl_odeiv.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/stat_log/stat_msg.h"


typedef H_DBL (*_fnc_1D)(H_DBL, void*);

typedef int (*_deriv_1D)(H_DBL, H_DBL*, H_DBL*, H_DBL*, int, int, void*);

typedef int (*_flag_crit)(void *, H_DBL *);


typedef struct
{
  int rank;

  /* void **C_data; */
  _fnc_1D *C_da;

  _deriv_1D *deriv;
  int ISN;
  
  _flag_crit *fc;

  const gsl_odeiv_step_type * step_T;
  
  void *params;
  
} h_fnc;



h_fnc *h_alloc_fnc ( void );

int h_init_fnc_cauchy_data ( h_fnc *f, void *params, int rank, ... );

int h_init_fnc_derivs ( h_fnc *f, int ISN, ... );

int h_init_fnc_flag_crit ( h_fnc *f, _flag_crit fc );

int h_init_fnc_step_type ( h_fnc *f, gsl_odeiv_step_type *s );

void h_free_fnc ( h_fnc *f );


#endif /* _FNC_COMMON_H_ */
