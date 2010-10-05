
#include "plot_1D.h"


/* void h_1Dplot_save_eps_gset ( h_gset * gset, int rank, int wghost, */
/*                               const char* title, int sleep_time ) */
/* { */
/*   char *fnc_msg = "1D plot gset"; */
  
/*   gnuplot_ctrl * handler = gnuplot_init(); */
  

/*   if ( gset == NULL ) { */
/*       _STAT_MSG ( fnc_msg, */
/*                   "h_gset is unallocated", */
/*                   WARNING, 0 ); */
      
/*       gnuplot_close( handler ); */
/*   } */
/*   else { */
      
/*       /\* setting default options for 1D plot *\/ */
/*       _h_1Dplot_set_options ( handler, title, gset->glevel[0]->grid[0]->t ); */

/*       /\* save guplot script *\/ */
/*       _h_1Dplot_save_eps_script ( gset->L ); */
/*       /\* save gset data/positions to files *\/ */
/*       _h_1Dplot_save_eps_gset ( gset, rank, wghost, handler ); */

/*       /\* if sleep_time is negative do not kill the plot window *\/ */
/*       /\* obtained by setting x11 terminal option persist *\/ */
/*       if ( sleep_time < 0 ) { */
/*           _h_1Dplot_add_options ( handler, "set term x11 persist" ); */
/*           gnuplot_cmd( handler, "load \"" FSCRIPT "\""); */
/*       } */
/*       else { */
/*           gnuplot_cmd( handler, "load \"" FSCRIPT "\""); */
/*           sleep( sleep_time ); */
/*       } */
      
/*       gnuplot_close( handler ); */
/*   } */
/* } */



/* void h_1Dplot_save_eps_glevel ( h_glevel * glevel, int rank, int wghost, */
/*                                 const char* title, int sleep_time ) */
/* { */
/*   char *fnc_msg = "1D plot glevel"; */
  
/*   gnuplot_ctrl * handler = gnuplot_init(); */
  

/*   if ( glevel == NULL ) { */
/*       _STAT_MSG ( fnc_msg, */
/*                   "h_glevel is unallocated", */
/*                   WARNING, 0 ); */
      
/*       gnuplot_close( handler ); */
/*   } */
/*   else { */
      
/*       /\* setting default options for 1D plot *\/ */
/*       _h_1Dplot_set_options ( handler, title, glevel->grid[0]->t ); */

/*       /\* save guplot script *\/ */
/*       _h_1Dplot_save_eps_script ( glevel->l ); */
/*       /\* save gset data/positions to files *\/ */
/*       _h_1Dplot_save_eps_glevel ( glevel, rank, wghost, handler ); */

/*       /\* if sleep_time is negative do not kill the plot window *\/ */
/*       /\* obtained by setting x11 terminal option persist *\/ */
/*       if ( sleep_time < 0 ) { */
/*           _h_1Dplot_add_options ( handler, "set term x11 persist" ); */
/*           gnuplot_cmd( handler, "load \"" FSCRIPT "\""); */
/*       } */
/*       else { */
/*           gnuplot_cmd( handler, "load \"" FSCRIPT "\""); */
/*           sleep( sleep_time ); */
/*       } */
      
/*       gnuplot_close( handler ); */
/*   } */
/* } */



void h_1Dplot_save_eps_gset ( h_gset * gset, int rank, int wghost,
                              const char *title, const char *file )
{
  char *fnc_msg = "1D plot save eps gset";
  
  gnuplot_ctrl * handler = gnuplot_init();
  
  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }
  else if ( file == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "empty eps file name",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }      
  else {
      
      /* Checking if the file exist */
      if ( access( file, F_OK ) != 0 ) {
          _STAT_MSG ( fnc_msg,
                      "file exists, overwriting",
                      WARNING, 0 );
      }


      /* setting default options for 1D plot */
      _h_1Dplot_set_options ( handler, title, gset->glevel[0]->grid[0]->t );

      /* save guplot script */
      _h_1Dplot_save_script ( gset->L );
      /* save gset data/positions to files */
      _h_1Dplot_save_gset ( gset, rank, wghost, handler );

      /* if sleep_time is negative do not kill the plot window */
      /* obtained by setting x11 terminal option persist */
      _h_1Dplot_add_options ( handler, "set term postscript enhanced color" );
      _h_1Dplot_add_options ( handler, "set output \"%s\"", file );

      gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
      
      gnuplot_close( handler );
  }
}



void h_1Dplot_save_eps_glevel ( h_glevel * glevel, int rank, int wghost,
                              const char *title, const char *file )
{
  char *fnc_msg = "1D plot save eps glevel";
  
  gnuplot_ctrl * handler = gnuplot_init();
  
  if ( glevel == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_glevel is unallocated",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }
  else if ( file == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "empty eps file name",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }      
  else {
      
      /* Checking if the file exist */
      if ( access( file, F_OK ) != 0 ) {
          _STAT_MSG ( fnc_msg,
                      "file exists, overwriting",
                      WARNING, 0 );
      }


      /* setting default options for 1D plot */
      _h_1Dplot_set_options ( handler, title, glevel->glevel[0]->grid[0]->t );

      /* save guplot script */
      _h_1Dplot_save_script ( glevel->L );
      /* save glevel data/positions to files */
      _h_1Dplot_save_glevel ( glevel, rank, wghost, handler );

      /* if sleep_time is negative do not kill the plot window */
      /* obtained by setting x11 terminal option persist */
      _h_1Dplot_add_options ( handler, "set term postscript enhanced color" );
      _h_1Dplot_add_options ( handler, "set output \"%s\"", file );

      gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
      
      gnuplot_close( handler );
  }
}



void h_1Dplot_save_eps_grid ( h_grid * grid, int rank, int wghost,
                              const char *title, const char *file )
{
  char *fnc_msg = "1D plot save eps grid";
  
  gnuplot_ctrl * handler = gnuplot_init();
  
  if ( grid == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_grid is unallocated",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }
  else if ( file == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "empty eps file name",
                  WARNING, 0 );
      
      gnuplot_close( handler );
  }      
  else {
      
      /* Checking if the file exist */
      if ( access( file, F_OK ) != 0 ) {
          _STAT_MSG ( fnc_msg,
                      "file exists, overwriting",
                      WARNING, 0 );
      }


      /* setting default options for 1D plot */
      _h_1Dplot_set_options ( handler, title, grid->t );

      /* save guplot script */
      _h_1Dplot_save_script ( grid->l );
      /* save gset data/positions to files */
      _h_1Dplot_save_grid ( grid, rank, wghost, handler );

      /* if sleep_time is negative do not kill the plot window */
      /* obtained by setting x11 terminal option persist */
      _h_1Dplot_add_options ( handler, "set term postscript enhanced color" );
      _h_1Dplot_add_options ( handler, "set output \"%s\"", file );

      gnuplot_cmd( handler, "load \"" FSCRIPT "\"");
      
      gnuplot_close( handler );
  }
}



