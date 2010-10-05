
#include "plot_1D.h"



void _h_1Dplot_save_script ( int L )
{
  FILE *fscript = fopen( FSCRIPT, "w");

  if ( fscript == NULL ) {
      perror ("fopen(fscript)");
      exit (EXIT_FAILURE);
  }

  /* fprintf ( fscript, "set terminal x11 persist\n"); */
  fprintf ( fscript, "set nokey\n");
  /* fprintf ( fscript, "set size 1.,0.55\n"); */
  fprintf ( fscript, "set origin 0,0\n");
  fprintf ( fscript, "set ylabel \"u(t, x)\"\n");
  fprintf ( fscript, "set xlabel \"\"\n");
  fprintf ( fscript, "set format x \"\"\n");
  
  fprintf ( fscript, "set multiplot\n");
  fprintf ( fscript, "set size 0.85,0.55\n");
  fprintf ( fscript, "set rmargin 0\n");
  fprintf ( fscript, "set lmargin 0\n");
  fprintf ( fscript, "set tmargin 0\n");
  fprintf ( fscript, "set bmargin 0\n");
  fprintf ( fscript, "set origin 0.1,0.35\n");

  fprintf ( fscript, "plot \""
            FDATA
            "\" u 1:2 with points lc rgb \"#ff6666\" ti \"\"\n" );
  
  fprintf ( fscript, "set size 0.85,0.25\n" );
  fprintf ( fscript, "set rmargin 0\n" );
  fprintf ( fscript, "set lmargin 0\n" );
  fprintf ( fscript, "set tmargin 0\n" );
  fprintf ( fscript, "set bmargin 0\n" );
  fprintf ( fscript, "set origin 0.1,0.1\n" );
  fprintf ( fscript, "unset title\n" );
  fprintf ( fscript, "unset ylabel\n" );
  fprintf ( fscript, "unset xlabel\n");
  fprintf ( fscript, "set xlabel \"x\"\n");
  fprintf ( fscript, "set ylabel \"l\"\n");
  fprintf ( fscript, "set format x \"%% g\"\n");

  if ( L < 7 )
      fprintf ( fscript, "set ytics 0,1,%d\n", L);
  else
      fprintf ( fscript, "set ytics 0,2,%d\n", L);
  
  fprintf ( fscript, "set key\n");
  fprintf ( fscript, "set grid x front\n");
  fprintf ( fscript, "set boxwidth 0.7\n");
  fprintf ( fscript, "set style fill solid 1.0 noborder\n");
  
  fprintf ( fscript, "plot [][:%d] \""
            FPOSITION
            "\" u 2:3 w filledcurve x1  lt 1 lc rgb \"#542222\"  ti \"gh\"\n", L+1);
  
  fprintf ( fscript, "replot \""
            FPOSITION "\" u 1:3 w filledcurve x1 lt 1 lc rgb \"#a84343\" ti \"no gh\"\n");

  fprintf ( fscript, "unset multiplot\n");


  fclose ( fscript );
}



void _h_1D_plot_save_grid_position ( h_grid *grid )
{
  int l;
  char *mode;
  H_DBL xL, xR, xLgh, xRgh;

  FILE *fposition;
  
  if ( grid->is_master == H_TRUE ) mode = "w";
  else mode = "a";

  fposition = fopen( FPOSITION, mode );
  if ( fposition == NULL ) {
      perror ("fopen(fposition)");
      exit (EXIT_FAILURE);
  }

  l = grid->l;
  
  xLgh = grid->x[0];
  xRgh = grid->x[grid->Ntotal-1];
  xL = grid->xL;
  xR = grid->xR;

  fprintf( fposition, "# l=%d m=%d\n", grid->l, grid->m );
  fprintf( fposition,
           "%f\t%f\t%d\n"
           "%f\t%f\t%d\n"
           "%f\t%f\t%d\n"
           "%f\t%f\t%d\n\n\n",
           xL, xLgh, l-1,
           xL, xLgh, l,
           xR, xRgh, l,
           xR, xRgh, l-1 );

  fclose ( fposition );
}



void _h_1D_plot_save_grid_data ( h_grid *grid, int rank, int wghost )
{
  int i, N;
  char *mode;
  H_DBL *x, *u;;

  FILE *fdata;
  
  if ( grid->is_master == H_TRUE ) mode = "w";
  else mode = "a";

  fdata = fopen( FDATA, mode );
  if ( fdata == NULL ) {
      perror ("fopen(fdata)");
      exit (EXIT_FAILURE);
  }

  if( wghost == H_TRUE ) { /* with ghost points */
      N = grid->Ntotal;
      x = h_get_grid_positions_wghosts( grid );
      u = h_get_grid_values_wghosts( grid, rank );
  }
  else { /* without ghost points */
      N = grid->N;
      x = h_get_grid_positions( grid );
      u = h_get_grid_values( grid, rank );
  }

  fprintf( fdata, "# l=%d m=%d N=%d Lghost=%d Rghost=%d Ntotal=%d\n",
           grid->l, grid->m, grid->N, grid->Lghost, grid->Rghost, grid->Ntotal );
  
  for (i = 0; i < N; i++) {
      fprintf( fdata, "%e\t%e\n", x[i], u[i] );
  }

  fclose ( fdata );
}



void _h_1Dplot_save_grid ( h_grid *grid, int rank, int wghost,
                           gnuplot_ctrl *handler )
{
  void *tmp;
  tmp = (void *) handler;
  _h_1D_plot_save_grid_position ( grid );
  _h_1D_plot_save_grid_data ( grid, rank, wghost );
}



void _h_1Dplot_save_glevel ( h_glevel *glevel, int rank, int wghost,
                             gnuplot_ctrl *handler )
{
  int m, M;

  M = glevel->M;
  
  for (m = 0; m < M; m++) {
      _h_1Dplot_save_grid ( glevel->grid[m], rank, wghost, handler ); 
  }
}



void _h_1Dplot_save_gset ( h_gset *gset, int rank, int wghost,
                           gnuplot_ctrl *handler )
{
  int l, L;

  L = gset->L;
  
  for (l = 0; l < L; l++) {
      _h_1Dplot_save_glevel ( gset->glevel[l], rank, wghost, handler ); 
  }
}



void h_1Dplot_save_gset ( h_gset * gset, int rank, int wghost,
                          const char* title, int sleep_time )
{
  char *fnc_msg = "1D plot gset";
  
  gnuplot_ctrl * handler = gnuplot_init();
  

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }
  else {
      
      /* setting default options for 1D plot */
      _h_1Dplot_set_options ( handler, title, gset->glevel[0]->grid[0]->t );

      /* save guplot script */
      _h_1Dplot_save_script ( gset->L );
      /* save gset data/positions to files */
      _h_1Dplot_save_gset ( gset, rank, wghost, handler );

      /* if sleep_time is negative do not kill the plot window */
      /* obtained by setting x11 terminal option persist */
      if ( sleep_time < 0 ) {
          _h_1Dplot_add_options ( handler, "set term x11 persist" );
          gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
      }
      else {
          gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
          sleep( sleep_time );
      }
      
      gnuplot_close( handler );
  }
}



void h_1Dplot_save_glevel ( h_glevel * glevel, int rank, int wghost,
                            const char* title, int sleep_time )
{
  char *fnc_msg = "1D plot glevel";
  
  gnuplot_ctrl * handler = gnuplot_init();
  

  if ( glevel == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_glevel is unallocated",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }
  else {
      
      /* setting default options for 1D plot */
      _h_1Dplot_set_options ( handler, title, glevel->grid[0]->t );

      /* save guplot script */
      _h_1Dplot_save_script ( glevel->l );
      /* save gset data/positions to files */
      _h_1Dplot_save_glevel ( glevel, rank, wghost, handler );

      /* if sleep_time is negative do not kill the plot window */
      /* obtained by setting x11 terminal option persist */
      if ( sleep_time < 0 ) {
          _h_1Dplot_add_options ( handler, "set term x11 persist" );
          gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
      }
      else {
          gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
          sleep( sleep_time );
      }
      
      gnuplot_close( handler );
  }
}



void h_1Dplot_save_grid ( h_grid * grid, int rank, int wghost,
                          const char* title, int sleep_time )
{
  char *fnc_msg = "1D plot grid";
  
  gnuplot_ctrl * handler = gnuplot_init();
  

  if ( grid == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_grid is unallocated",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }
  else {
      
      /* setting default options for 1D plot */
      _h_1Dplot_set_options ( handler, title, grid->t );

      /* save guplot script */
      _h_1Dplot_save_script ( grid->l );
      /* save gset data/positions to files */
      _h_1Dplot_save_grid ( grid, rank, wghost, handler );

      /* if sleep_time is negative do not kill the plot window */
      /* obtained by setting x11 terminal option persist */
      if ( sleep_time < 0 ) {
          _h_1Dplot_add_options ( handler, "set term x11 persist" );
          gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
      }
      else {
          gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
          sleep( sleep_time );
      }
      
      gnuplot_close( handler );
  }
}



