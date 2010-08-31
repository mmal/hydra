#ifndef _H_PLOT_1D_H_
#define _H_PLOT_1D_H_

#include <stdio.h>
#include <string.h>


#include "src/plot/gnuplot_i.h"
#include "src/common/h_types.h"
#include "src/h_grids_common.h"
#include "src/error_log/error_printf.h"


void _h_1D_set_plot_options ( gnuplot_ctrl * h, const char * title,
                              const H_DBL time );

void h_plot_1D_one_grid ( const h_grid * g, const char* title,
                          int sleep_time );

void h_plot_1D_grid_family ( const h_grid * cg, int lmax, char* title,
                             int sleep_time );

#endif /* _H_PLOT_1D_H_ */
