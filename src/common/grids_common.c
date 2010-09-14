

#include "grids_common.h"


h_grid * h_alloc_grid ( void )
{
  h_grid * g = (h_grid*) malloc ( sizeof( h_grid ) );

  if ( g == NULL )
      _STAT_MSG ( "Allocating grid",
                  "cannot allocate grid",
                  ERROR, 0 );
  else {
      g->x = NULL;
      g->u = NULL;
      g->master = NULL;
      g->offspring = NULL;
      g->sibling = NULL;
      g->parent = NULL;
      g->neighbour = NULL;
      g->Lsibling = NULL;
      g->Rsibling = NULL;
      g->children = NULL;
      g->is_master = H_FALSE;
      
      _STAT_MSG ( "Allocating grid",
                  NULL,
                  OK, 0 );
  }
  return g;
}


void h_init_grid ( h_grid * g, H_DBL xL, H_DBL xR,
                   int N, int Lghost, int Rghost,
                   int rank, int l, int m )
{
  int i;

  char * fnc_msg = (char*) malloc( 35*sizeof(char) );

  sprintf( fnc_msg, "Initializing grid l=%d, m=%d", l, m );
  
  if ( g == NULL ) 
      _STAT_MSG ( fnc_msg,
                  "cannot initialize non allocated grid",
                  ERROR, 0 );

  if ( N<2 || rank<0 )
      _STAT_MSG ( fnc_msg,
                  "number of grid points must be >2 and equation order " \
                  "must be >0",
                  ERROR, 0 );

  if ( xR<=xL )
      _STAT_MSG ( fnc_msg,
                  "for all grids xR must be greater than xL",
                  ERROR, 0 );

  g->rank = rank;
  g->xL = xL;
  g->xR = xR;
  g->h = (xR-xL)/(N-1);
  g->N = N;
  g->Lghost = Lghost;
  g->Rghost = Rghost;

  g->l = l;
  g->m = m;
    
  g->t = 0.;

  g->x = ( H_DBL *) malloc ( (N+Lghost+Rghost)*sizeof( H_DBL ) );

  if (g->x == NULL ) {
      free ( g );
      _STAT_MSG ( fnc_msg,
                  "cannot allocate memory for grid points",
                  ERROR, 0 );
  }
  
  if ( Lghost+Rghost==0 ) {  /* Lghost=Rghost=0*/
      for (i = 0; i < N; i++) {
          g->x[i] = xL + i*(xR-xL)/(N-1);
      }
  }
  else { 
      for (i = 0; i < Lghost; i++) {
          g->x[i] = xL  - (Lghost-i)*g->h;
      }
      for (i = 0; i < N; i++)  {
          g->x[i+Lghost] = xL + i*g->h;
      }
      for (i = 0; i < Rghost; i++) {
          g->x[N+Lghost+i] = xR + (i+1)*g->h;
      }
  }

  g->u = ( H_DBL *) malloc ( rank*(N+Lghost+Rghost)*sizeof( H_DBL ) );

  if (g->u == NULL ) {
      free ( g->x );
      free ( g );
      _STAT_MSG ( fnc_msg,
                  "cannot allocate memory for grid data",
                  ERROR, 0 );
  }

  g->master = NULL;
  
  g->offspring = NULL;
  g->sibling = NULL;
  g->parent = NULL;
  g->neighbour = NULL;
  g->children = NULL;

  
  _STAT_MSG ( fnc_msg,
              NULL,
              OK, 0 );

  free( fnc_msg );
}


void h_init_coarse_grid ( h_grid * g, H_DBL xL, H_DBL xR,
                          int N, int rank )
{
  h_init_grid ( g, xL, xR, N, 0, 0, rank, 0, 0 );
  g->master = NULL;
  g->is_master = H_TRUE;
  g->Nchildren = 0;
}


H_DBL *h_get_grid_positions ( h_grid *g )
{
  int Lghost = g->Lghost;
  /* H_DBL *xptr = g->x+Lghost; */

  /* xptr+=Lghost; */

  return g->x+Lghost;
}


H_DBL *h_get_grid_positions_wghosts ( h_grid *g )
{
  return g->x;
}


H_DBL *h_get_grid_values ( h_grid *g, int rank )
{
  int N = g->N;
  int Lghost = g->Lghost;
  int Rghost = g->Rghost;
  int Ntotal = N+Lghost+Rghost;

  /* H_DBL *uptr = &g->u[0]; */

  /* uptr+=rank*Ntotal+Lghost; */

  return g->u+rank*Ntotal+Lghost;
}


H_DBL *h_get_grid_values_wghosts ( h_grid *g, int rank )
{ 
  int N = g->N;
  int Lghost = g->Lghost;
  int Rghost = g->Rghost;
  int Ntotal = N+Lghost+Rghost;

  /* H_DBL *uptr = &g->u[0]; */

  /* uptr+=rank*Ntotal; */

  return g->u+rank*Ntotal;
}


void h_free_grid ( h_grid * g )
{
  char *fnc_msg;
  
  if ( g != NULL ) {
      if ( g->x != NULL )
          free ( g->x );

      if ( g->u != NULL )
          free ( g->u );

      if ( g->children != NULL ) {
          for (int i = 0; i < g->Nchildren; i++) {
              h_free_grid ( g->children[i] );
          }
          free ( g->children );
      }
      fnc_msg = (char*) malloc( 35*sizeof(char) );
      sprintf(fnc_msg, "Feeing grid l=%d, m=%d", g->l, g->m);
      free( g );
      g = NULL;
      _STAT_MSG ( fnc_msg,
                  NULL,
                  OK, 0 );
      free( fnc_msg );
  }
  else
      _STAT_MSG ( "Feeing grid",
                  "grid was not allocated",
                  WARNING, 0 );
}
