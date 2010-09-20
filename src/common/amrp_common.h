#ifndef _H_AMRP_H_
#define _H_AMRP_H_


#include <argtable2.h>


#include "src/common/types.h"
#include "src/stat_log/stat_msg.h"


typedef struct
{
  int rr;                       /**< refinement ratio */
  int buf;                      /**< buffer size */
  int sp;                       /**< stencil points */
  int lmax;                     /**< number of grids levels */
  int lmbd;                     /**< CFL condition constant */

  int ngh;                      /**< number of ghost cells */
  
  int gme;                      /**< grids move every */
  
  H_DBL errt;                   /**< error tolerance */
  
} h_amrp;


h_amrp *h_alloc_amrp ( void );

void h_init_amrp ( h_amrp *p, int argc, char *argv[] );

void h_free_amrp ( h_amrp *p );

#endif /* _H_AMRP_H_ */
