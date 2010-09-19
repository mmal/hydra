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

#define Fdata "data.dat"
#define Fgrid "positions.dat"


void _h_1D_set_plot_options ( gnuplot_ctrl * h, const char * title,
                              const H_DBL time )
{
  int title_len;
  char *set_title;

  if ( title != NULL ) {
      title_len = strlen( title );
      set_title = (char*) malloc( (28+title_len)*sizeof( char ) );
      sprintf(set_title, "set title \"%s, t=%.5e\"", title, time);
  }
  else {
      set_title = (char*) malloc( 28*sizeof( char ) );
      sprintf(set_title, "set title \"t=%.5e\"", time);
  }

  
  gnuplot_cmd( h, set_title);
  gnuplot_cmd( h, "set xlabel \"x\"; set ylabel \"u(t, x)\"");
  gnuplot_cmd( h, "set term x11" );

  gnuplot_cmd( h, "set style data lines");
  gnuplot_cmd( h, "unset key");

  free( set_title );
}


void _h_1D_add_plot_options ( gnuplot_ctrl * h, const char *option )
{
  if ( option != NULL )
      gnuplot_cmd( h, option );
}


void h_1D_plot_one_grid ( const h_grid * g, int rank, int wghosts,
                          const char* title, int sleep_time )
{
  int N;
  H_DBL *x, *u;
  
  gnuplot_ctrl * handler = gnuplot_init();

  if( wghosts == H_TRUE ) { /* with ghost points */
      N = g->N+g->Lghost+g->Rghost;
      x = h_get_grid_positions_wghosts(g);
      u = h_get_grid_values_wghosts(g, rank);
  }
  else { /* without ghost points */
      N = g->N;
      x = h_get_grid_positions(g);
      u = h_get_grid_values(g, rank);
  }
  
  _h_1D_set_plot_options ( handler, title, g->t ); /* setting default
                                                    * options for 1D
                                                    * plot
                                                    */
  
  if ( sleep_time < 0 ) { /* if sleep_time is negative
                           * do not kill the plot window
                           * obtained by setting x11 terminal option 'persist'
                           */
      _h_1D_add_plot_options ( handler, "set term x11 persist" );
      gnuplot_plot_xy( handler, x, u, N, NULL );
  }
  else {
      gnuplot_plot_xy( handler, x, u, N, NULL );
      sleep( sleep_time );
  }

  gnuplot_close( handler );
}


void _h_1D_plot_set_of_grids_recursion ( const h_grid * g,
                                         gnuplot_ctrl * handler )
{
  int m;
  int N;
  int rank=0;
  H_DBL *x, *u;
  
  h_grid **child;

  int wghosts = H_TRUE;
  
  if( wghosts == H_TRUE ) { /* with ghost points */
      N = g->N+g->Lghost+g->Rghost;
      x = h_get_grid_positions_wghosts(g);
      u = h_get_grid_values_wghosts(g, rank);
  }
  else { /* without ghost points */
      N = g->N;
      x = h_get_grid_positions(g);
      u = h_get_grid_values(g, rank);
  }

  
  gnuplot_plot_xy( handler, x, u, N, NULL );
  

  /* if ( g->children == NULL ) */
  /*     printf("\n\n g->children == NULL \n\n"); */
  /* else */
  /*     printf("\n\n g->children != NULL \n\n"); */
  
  if ( g->children != NULL ) {
      child = (h_grid **) g->children;
  
      for (m = 0; m < g->Nchildren; m++) {
          _h_1D_plot_set_of_grids_recursion ( child[m], handler );
          
      }
  }
}


void _h_1D_save_set_of_grids_recursion ( h_grid *g, int rank, int wghosts )
{
  int i;
  int m;
  int N;
  int l = g->l;
 
  char *mode;

  H_DBL *x, *u, xL, xR, xLgh, xRgh;
  
  FILE *fdata; 
  FILE *fpos;

  h_grid **child;


  if ( g->is_master == H_TRUE )
      mode = "w";
  else
      mode = "a";
  
  fdata = fopen( "data.dat", mode );
  fpos = fopen( "position.dat", mode );
      
  
  if( wghosts == H_TRUE ) { /* with ghost points */
      N = g->N+g->Lghost+g->Rghost;
      x = h_get_grid_positions_wghosts(g);
      u = h_get_grid_values_wghosts(g, rank);
  }
  else { /* without ghost points */
      N = g->N;
      x = h_get_grid_positions(g);
      u = h_get_grid_values(g, rank);
  }
  
  xLgh = g->x[0];
  xRgh = g->x[g->N+g->Lghost+g->Rghost-1];
  xL = g->xL;
  xR = g->xR;

  VL(( "Lghost=%d, Rghost=%d, N=%d\n", g->Lghost, g->Rghost, N ));
  
  VL(("l=%d xL=%f xLgh=%f xR=%f xRgh=%f\n", l, xL, xLgh, xR, xRgh ));

  fprintf( fpos, "%f\t%f\t%d\n" "%f\t%f\t%d\n" "%f\t%f\t%d\n" "%f\t%f\t%d\n\n\n",
           xL, xLgh, l-1, xL, xLgh, l, xR, xRgh, l, xR, xRgh, l-1 );

  
  fclose ( fpos );
  
  for (i = 0; i < N; i++) {
      fprintf( fdata, "%e\t%e\n", x[i], u[i] );
  }

  fclose ( fdata );
  
  if ( g->children != NULL ) {
      child = (h_grid **) g->children;
  
      for (m = 0; m < g->Nchildren; m++) {
          _h_1D_save_set_of_grids_recursion ( child[m], rank, wghosts );
      }
  }
}


void h_1D_plot_set_of_grids ( const h_grid * cg, int lmax, const char* title,
                                int sleep_time )
{
  int l, m;
  
  gnuplot_ctrl * handler = gnuplot_init();
  
  _h_1D_set_plot_options ( handler, title, cg->t ); /* setting default
                                                    * options for 1D
                                                    * plot
                                                    */
  
  if ( sleep_time < 0 ) { /* if sleep_time is negative
                           * do not kill the plot window
                           * obtained by setting x11 terminal option 'persist'
                           */
      _h_1D_add_plot_options ( handler, "set term x11 persist" );
      _h_1D_plot_set_of_grids_recursion ( cg, handler );
  }
  else {
      _h_1D_plot_set_of_grids_recursion ( cg, handler );
      sleep( sleep_time );
  }

  gnuplot_close( handler );
}



void h_1D_plot_set_of_grids_2 ( const h_grid * cg, int rank, int wghosts,
                              const char* title, int sleep_time )
{
  int l, m;
  
  gnuplot_ctrl * handler = gnuplot_init();
  
  _h_1D_set_plot_options ( handler, title, cg->t ); /* setting default
                                                    * options for 1D
                                                    * plot
                                                    */
  
  if ( sleep_time < 0 ) { /* if sleep_time is negative
                           * do not kill the plot window
                           * obtained by setting x11 terminal option 'persist'
                           */
      /* _h_1D_add_plot_options ( handler, "set term x11 persist" ); */
      
      
      _h_1D_save_set_of_grids_recursion ( cg, rank, wghosts );
      
      gnuplot_cmd( handler, "load \"script.gp\"");

      
      /* _h_1D_plot_set_of_grids_recursion ( cg, handler ); */
  }
  else {
      _h_1D_plot_set_of_grids_recursion ( cg, handler );
      sleep( sleep_time );
  }

  gnuplot_close( handler );
}

