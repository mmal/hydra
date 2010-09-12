#ifndef _PLOT_1D_H_
#define _PLOT_1D_H_


#include <stdio.h>
#include <string.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/stat_log/stat_msg.h"
#include "src/plot_data/gnuplot_i.h"


void _h_1D_set_plot_options ( gnuplot_ctrl * h, const char * title,
                              const H_DBL time );

void h_1D_plot_one_grid ( const h_grid * g, const char* title,
                          int sleep_time );

void h_1D_plot_set_of_grids ( const h_grid * cg, int lmax, char* title,
                              int sleep_time );


#endif /* _PLOT_1D_H_ */