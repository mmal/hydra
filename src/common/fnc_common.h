#ifndef _FNC_COMMON_H_
#define _FNC_COMMON_H_


#include <stdarg.h>


#include "src/common/types.h"
#include "src/stat_log/stat_msg.h"


typedef H_DBL (*_fnc_1D)(H_DBL, void*);

    
typedef struct
{
  int rank;

  /* void **C_data; */
  _fnc_1D *C_da;
  
  void *params;
  
} h_fnc;



h_fnc *h_alloc_fnc ( void *params, int rank, ... );

void h_free_fnc ( h_fnc *f );

#endif /* _FNC_COMMON_H_ */
