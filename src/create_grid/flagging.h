#ifndef _FLAGGING_H_
#define _FLAGGING_H_


#include "src/common/common.h"


void h_flagging_points ( h_grid *grid, h_amrp *amrp, h_fnc *fnc,
                         int **id_fp, int *Nfp );

void h_clustering_flagged ( int *id_fp , int Nfp, int buf, int Ncoarse,
                            int **idL, int **idR, int *Ngrids );


#endif /* _FLAGGING_H_ */
