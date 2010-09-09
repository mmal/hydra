#ifndef _FLAG_CRITERION_H_
#define _FLAG_CRITERION_H_


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"



int h_fc_Richardson ( h_grid * g, h_amrp * p, H_DBL * tau );

int h_fc_Energy ( h_grid * g, h_amrp * p, H_DBL * tau );

int h_fc_SV ( h_grid * g, h_amrp * p, H_DBL * tau );

int h_fc_TV ( h_grid * g, h_amrp * p, H_DBL * tau );

  
#endif /* _FLAG_CRITERION_H_ */
