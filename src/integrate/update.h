#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/hms_common.h"



int _h_update_grid ( h_grid *parent, h_grid *child  );
int _h_update_glevel ( h_glevel *parent, h_glevel *child  );


#endif /* _UPDATE_H_ */
