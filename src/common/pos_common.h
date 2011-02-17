#ifndef _POS_COMMON_H_
#define _POS_COMMON_H_


#include <stdlib.h>
#include <stdio.h>


#include "src/common/types.h"
#include "src/stat_log/stat_msg.h"


typedef struct
{
  int iLout;
  int iLin;

  int iRin;
  int iRout;

  H_DBL xLout;
  H_DBL xLin;

  H_DBL xRin;
  H_DBL xRout;
  
} h_pos;

h_pos *h_alloc_pos ( void );
void h_free_pos ( h_pos *p );


#endif /* _POS_COMMON_H_ */
