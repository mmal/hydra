#ifndef _FLAGGING_H_
#define _FLAGGING_H_


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"



void h_flagging_points ( h_grid *g, h_amrp *p,
                         int (*flagging_criterion)( h_grid *, h_amrp *, H_DBL * ),
                         int **id_fp, int *Nfp );

void h_clustering_flagged ( int *id_fp , int Nfp, int buf, int Ncoarse,
                            int **idL, int **idR, int *Ngrids );


#endif /* _FLAGGING_H_ */
