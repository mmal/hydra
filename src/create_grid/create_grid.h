#ifndef _H_GENERATE_GRID_H_
#define _H_GENERATE_GRID_H_


#include "src/common/common.h"

#include "src/create_grid/flagging.h"
#include "src/create_grid/flag_criterion.h"


void _h_create_child_grid ( h_grid *parent, h_grid *child,
                            h_amrp *p, int m, int idL, int idR );
int h_create_init_gset ( h_hms *hms );
int h_create_init_glevel ( h_hms *hms, int l );
int h_create_init_grid ( h_grid *pgrid, h_glevel *chglevel,
                         h_amrp *amrp, h_fnc *fnc );
void _h_acd_to_grid ( h_grid *g, h_fnc *f );
void _h_acd_to_glevel ( h_glevel *glevel, h_fnc *fnc );
void _h_acd_to_gset ( h_gset *gset, h_fnc *fnc );

#endif /* _H_GENERATE_GRID_H_ */
