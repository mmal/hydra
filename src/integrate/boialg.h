#ifndef _BOIALG_H_
#define _BOIALG_H_

#include <math.h>

#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/hms_common.h"

#include "src/integrate/step.h"
#include "src/integrate/update.h"



int h_boialg ( h_hms *hms );
int _h_boialg_old ( h_gset *gset, h_amrp *amrp, h_fnc *fnc, int l );
int _h_boialg ( h_gset *gset, h_amrp *amrp, h_fnc *fnc, int l );


#endif /* _BOIALG_H_ */
