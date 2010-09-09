/**
 * @file   plot_1D.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Sun Aug 29 14:38:40 2010
 * 
 * @brief  Plotting functions for 1D grids.
 * 
 * 
 */


#include "plot_1D.h"


void _h_1D_set_plot_options ( gnuplot_ctrl * h, const char * title,
                              const H_DBL time )
{
  int title_len;
  char *set_title;

  if ( title != NULL ) {
      title_len = strlen( title );
      set_title = (char*) malloc( (27+title_len)*sizeof( char ) );
      sprintf(set_title, "set title \"%s, t=%.5e\"", title, time);
  }
  else {
      set_title = (char*) malloc( 27*sizeof( char ) );
      sprintf(set_title, "set title \"t=%.5e\"", time);
  }

  
  gnuplot_cmd( h, set_title);
  gnuplot_cmd( h, "set xlabel \"x\"; set ylabel \"u(t, x)\"");
  gnuplot_cmd( h, "set term x11" );

  gnuplot_cmd( h, "set style data lines");
  gnuplot_cmd( h, "unset key");

  free( set_title );
}


void h_1D_plot_one_grid ( const h_grid * g, const char* title, int sleep_time )
{
  gnuplot_ctrl * handler = gnuplot_init();

  _h_1D_set_plot_options ( handler, title, g->t );

  gnuplot_plot_xy( handler, g->x, g->u, g->N, NULL );

  sleep( sleep_time );

  gnuplot_close( handler );
}

            
void h_1D_plot_set_of_grids ( const h_grid * cg, int lmax, char* title,
                              int sleep_time )
{
  int l;
  
  gnuplot_ctrl * handler = gnuplot_init();

  h_grid * g = cg;
  
  _h_1D_set_plot_options ( handler, title, g->t );

  for (l = 0; l < lmax; l++) {
      gnuplot_plot_xy( handler, g->x, g->u, g->N, NULL );
      
      g = (h_grid*) g->offspring;
  }

  sleep( sleep_time );

  gnuplot_close( handler );
}
