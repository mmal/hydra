#ifndef _FLAG_TEST_H_
#define _FLAG_TEST_H_

#include <stdio.h>
#include <stdarg.h>
#include <math.h>


/* #include <nana.h> */


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"
#include "src/stat_log/stat_msg.h"


#include "src/derivs_fda/derivs.h"
#include "src/ivp/ivp_1D.h"

#include "src/plot_data/plot_1D.h"

#include "src/create_grid/flagging.h"
#include "src/create_grid/flag_criterion.h"
#include "src/create_grid/create_grid.h"



H_DBL zero_ ( H_DBL x, void *params );
H_DBL one_ ( H_DBL x, void *params );
    
#endif /* _FLAG_TEST_H_ */
