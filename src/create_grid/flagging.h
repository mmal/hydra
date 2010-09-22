#ifndef _FLAGGING_H_
#define _FLAGGING_H_


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/common/fnc_common.h"
#include "src/common/hms_common.h"




void h_flagging_points ( h_hms *m, int **id_fp, int *Nfp );

void h_clustering_flagged ( int *id_fp , int Nfp, int buf, int Ncoarse,
                            int **idL, int **idR, int *Ngrids );


#endif /* _FLAGGING_H_ */
