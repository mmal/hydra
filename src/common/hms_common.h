#ifndef _HMS_COMMON_H_
#define _HMS_COMMON_H_


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"
#include "src/stat_log/stat_msg.h"


typedef struct
{

  h_grid *g;

  h_amrp *p;

  h_fnc *f;

} h_hms_old;


typedef struct
{

  h_gset *gset;                 /**< grids set */
  h_amrp *amrp;                 /**< adaptive mesh refinement
                                 * algorithm parameters */
  h_fnc *fnc;                   /**< function structure */

} h_hms;



h_hms *h_alloc_hms ( void );

void h_free_hms ( h_hms *m );


#endif /* _HMS_COMMON_H_ */
