#ifndef _H_AMRP_H_
#define _H_AMRP_H_

#include "src/common/h_types.h"
#include "src/error_log/error_printf.h"


typedef struct
{
  int rr;                       /**< refinement ratio */
  int buf;                      /**< buffer size */
  int sp;                       /**< stencil points */
  int lmax;                     /**< number of grids levels */
  int lmbd;                     /**< CFL condition constant */

  int gme;                      /**< grids move every */
  
  H_DBL errt;                   /**< error tolerance */
  
} h_amrp;


h_amrp * h_alloc_amrp ( void );

void h_free_amrp ( h_amrp * p );

#endif /* _H_AMRP_H_ */
