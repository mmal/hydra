#ifndef _FNC_COMMON_H_
#define _FNC_COMMON_H_


#include <stdarg.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/stat_log/stat_msg.h"


typedef H_DBL (*_fnc_1D)(H_DBL, void*);

typedef H_DBL (*_deriv_1D)(H_DBL, void*);

typedef int (*_flag_crit)(h_grid *, h_amrp *, H_DBL *);


typedef struct
{
  int rank;

  /* void **C_data; */
  _fnc_1D *C_da;

  _deriv_1D *deriv;

  _flag_crit fc;
  
  void *params;
  
} h_fnc;



h_fnc *h_alloc_fnc ( void *params, int rank, ... );


void h_init_fnc_Cdata ( int rank, ... );

void h_init_fnc_derivs ( int rank, ... );

void h_free_fnc ( h_fnc *f );

#endif /* _FNC_COMMON_H_ */
