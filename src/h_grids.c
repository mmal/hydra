#include "h_grids_common.h"


h_grid * h_alloc_grid ( void )
{
  h_grid * g = (h_grid*) malloc ( sizeof( h_grid ) );

  if ( g == NULL )
      _STAT_MSG ( "Allocating grid",
                  "cannot allocate grid",
                  ERROR, 0 );
  else
      _STAT_MSG ( "Allocating grid",
                  NULL,
                  OK, 0 );
  return g;
}


void h_init_coarse_grid ( h_grid * g, H_DBL xL, H_DBL xR,
                          int N, int ord )
{
  int i;
  
  if ( g == NULL ) 
      _STAT_MSG ( "Initializing coarse grid",
                  "cannot initialize non allocated grid",
                  ERROR, 0 );

  if ( N<2 || ord<0 )
      _STAT_MSG ( "Initializing coarse grid",
                  "number of grid points must be >2 and equation order " \
                  "must be >0",
                  ERROR, 0 );

  if ( xR<=xL )
      _STAT_MSG ( "Initializing coarse grid",
                  "for all grids xR must be greater than xL",
                  ERROR, 0 );
  
  g->ord = ord;
  g->xL = xL;
  g->xR = xR;
  g->h = (xR-xL)/(N-1);
  g->N = N;
  g->l = 0;
  g->m = 0;
    
  g->t = 0.;
  
  g->x = ( H_DBL *) malloc ( N*sizeof( H_DBL ) );
  if (g->x == NULL ) {
      free ( g );
      _STAT_MSG ( "Initializing coarse grid",
                  "cannot allocate memory for grid points",
                  ERROR, 0 );
  }

  for (i = 0; i < N; i++) {
      g->x[i] = xL + i*(xR-xL)/(N-1);
  }

  g->u = ( H_DBL *) malloc ( ord*N*sizeof( H_DBL ) );
  if (g->u == NULL ) {
      free ( g->x );
      free ( g );
      _STAT_MSG ( "Initializing coarse grid",
                  "cannot allocate memory for grid data",
                  ERROR, 0 );
  }

  g->offspring = NULL;
  g->sibling = NULL;
  g->parent = NULL;
  g->neighbour = NULL;

  _STAT_MSG ( "Initializing coarse grid",
              NULL,
              OK, 0 );
}


void h_init_fine_grid ( h_grid * g, H_DBL xL, H_DBL xR,
                        int N, int ord, int l, int m )
{
  int i;

  char * fnc_msg = (char*) malloc( 35*sizeof(char) );

  sprintf( fnc_msg, "Initializing fine grid l=%d, m=%d", l, m );
  
  if ( g == NULL ) 
      _STAT_MSG ( fnc_msg,
                  "cannot initialize non allocated grid",
                  ERROR, 0 );

  if ( N<2 || ord<0 )
      _STAT_MSG ( fnc_msg,
                  "number of grid points must be >2 and equation order " \
                  "must be >0",
                  ERROR, 0 );

  if ( xR<=xL )
      _STAT_MSG ( fnc_msg,
                  "for all grids xR must be greater than xL",
                  ERROR, 0 );
  
  g->ord = ord;
  g->xL = xL;
  g->xR = xR;
  g->h = (xR-xL)/(N-1);
  g->N = N;
  g->l = l;
  g->m = m;
    
  g->t = 0.;
  
  g->x = ( H_DBL *) malloc ( N*sizeof( H_DBL ) );
  if (g->x == NULL ) {
      free ( g );
      _STAT_MSG ( fnc_msg,
                  "cannot allocate memory for grid points",
                  ERROR, 0 );
  }

  for (i = 0; i < N; i++) {
      g->x[i] = xL + i*(xR-xL)/(N-1);
  }

  g->u = ( H_DBL *) malloc ( ord*N*sizeof( H_DBL ) );
  if (g->u == NULL ) {
      free ( g->x );
      free ( g );
      _STAT_MSG ( fnc_msg,
                  "cannot allocate memory for grid data",
                  ERROR, 0 );
  }

  g->offspring = NULL;
  g->sibling = NULL;
  g->parent = NULL;
  g->neighbour = NULL;

  _STAT_MSG ( fnc_msg,
              NULL,
              OK, 0 );

  free( fnc_msg );
}



void h_free_grid ( h_grid * g )
{
  if ( g != NULL ) {
      if ( g->x != NULL )
          free (g->x );

      if ( g->u != NULL )
          free (g->u );

      free( g );
      _STAT_MSG ( "Feeing grid",
                  NULL,
                  OK, 0 );
  }
  
  else
      _STAT_MSG ( "Feeing grid",
                  "grid was not allocated",
                  WARNING, 0 );
}
