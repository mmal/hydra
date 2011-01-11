#ifndef _PLOT_1D_H_
#define _PLOT_1D_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include "src/common/types.h"
#include "src/common/grids_common.h"
#include "src/common/amrp_common.h"
#include "src/stat_log/stat_msg.h"
#include "src/plot_data/gnuplot_i.h"

/* #define FDATA "data.dat" */
/* #define FPOSITION "positions.dat" */
/* #define FSCRIPT "script.gp" */



#define MAX_CHAR 1024

void _h_1Dplot_gen_names ( void );
void _h_1Dplot_set_options ( gnuplot_ctrl * h, const char * title, const H_DBL time );
void _h_1Dplot_add_options ( gnuplot_ctrl * h, const char *option, ... );
void _h_1Dplot_grid ( h_grid *grid, int rank, int wghost, gnuplot_ctrl *handler );
void _h_1Dplot_glevel ( h_glevel *glevel, int rank, int wghost, gnuplot_ctrl *handler );
void _h_1Dplot_gset ( h_gset *gset, int rank, int wghost, gnuplot_ctrl *handler );
void h_1Dplot_grid ( h_grid *grid, int rank, int wghost, const char* title, int sleep_time );
void h_1Dplot_glevel ( h_glevel * glevel, int rank, int wghost, const char* title, int sleep_time );
void h_1Dplot_gset ( h_gset * gset, int rank, int wghost, const char* title, int sleep_time );

void _h_1Dplot_save_script ( int L );
void _h_1D_plot_save_grid_position ( h_grid *grid );
void _h_1D_plot_save_grid_data ( h_grid *grid, int rank, int wghost );
void _h_1Dplot_save_grid ( h_grid *grid, int rank, int wghost, gnuplot_ctrl *handler );
void _h_1Dplot_save_glevel ( h_glevel *glevel, int rank, int wghost, gnuplot_ctrl *handler );
void _h_1Dplot_save_gset ( h_gset *gset, int rank, int wghost, gnuplot_ctrl *handler );
void h_1Dplot_save_gset ( h_gset * gset, int rank, int wghost, const char* title, int sleep_time );
void h_1Dplot_save_grid ( h_grid * grid, int rank, int wghost, const char* title, int sleep_time );
void h_1Dplot_save_glevel ( h_glevel * glevel, int rank, int wghost, const char* title, int sleep_time );

void h_1Dplot_save_eps_grid ( h_grid * grid, int rank, int wghost, const char *title, const char *file );
void h_1Dplot_save_eps_glevel ( h_glevel * glevel, int rank, int wghost, const char *title, const char *file );
void h_1Dplot_save_eps_gset ( h_gset * gset, int rank, int wghost, const char *title, const char *file );


#endif /* _PLOT_1D_H_ */
