#ifndef _INTERPOLATE_H_
#define _INTERPOLATE_H_

/* #include <float.h> */
/* #include <math.h> */

#include "src/common/types.h"
#include "src/common/grids_common.h"
/* #include "src/derivs_fda/derivs.h" */

/* #include "src/common/amrp_common.h" */
/* #include "src/common/hms_common.h" */

H_DBL *_h_compute_D2 (H_DBL xa[], H_DBL ya[], int n);

void _h_splint (H_DBL xa[], H_DBL ya[], H_DBL y2a[], int n, H_DBL x, H_DBL *y );

void _h_interpolate_from_parent_to_child_grid ( h_grid *parent, h_grid *child );

#endif /* _INTERPOLATE_H_ */
