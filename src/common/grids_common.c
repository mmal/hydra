

#include "grids_common.h"


h_grid * h_alloc_grid ( void )
{
  h_grid * g = (h_grid*) malloc ( sizeof( h_grid ) );

  if ( g == NULL )
      _STAT_MSG ( "Allocating grid",
                  "cannot allocate grid",
                  ERROR, 0 );
  else {
      g->N = 0;
      g->l = 0;
      g->m = 0;
      g->rank = 0;
      g->Nchildren = 0;
      g->Ncalls = 0;
      g->Lghost = 0;
      g->Rghost = 0;
      g->Ntotal = 0;

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

      /* _STAT_MSG ( "Allocating grid", */
      /*             NULL, */
      /*             OK, 0 ); */
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
  VL((" *** m = %d", m));
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
  g->Ntotal = N+Lghost+Rghost;
  
  g->l = l;
  g->m = m;
    
  g->t = 0.;
  g->tlast = 0.;
  g->Ncalls = 0;
  
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

  /* g->master = NULL; */
  
  /* g->offspring = NULL; */
  /* g->sibling = NULL; */
  /* g->parent = NULL; */
  /* g->neighbour = NULL; */
  /* g->children = NULL; */

  
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

/* TODO:
 *  free h_free_rem_glevel
 *  free h_free_rem_grid
 */

/** 
 * allocates memory for a h_gset structure containing
 * one level consisting of one grid (master grid),
 * and returns a pointer to the allocated memory space.
 * 
 * @return pointer to the h_gset struct
 */
h_gset *h_alloc_gset ( void )
{
  /* TODO: */
  /* check whether there are errors during memory allocation  */
  int l, m;
  int L = 1; /* # of levels */
  int M = 1; /* # of grids in each level */

  h_gset * gset = (h_gset*) malloc ( sizeof( h_gset ) );

  gset->L = L;
  gset->glevel = (h_glevel**) malloc ( L*sizeof( h_glevel* ) );

  for (l = 0; l < L; l++) {
      gset->glevel[l] = (h_glevel*) malloc ( sizeof( h_glevel ) );

      gset->glevel[l]->l = l;
      gset->glevel[l]->M = M;
      
      gset->glevel[l]->grid = (h_grid**) malloc ( M*sizeof( h_grid* ) );

      for (m = 0; m < M; m++) {
          gset->glevel[l]->grid[m] = (h_grid*) malloc ( sizeof( h_grid ) );
      }
  }
  
  return gset;
}



/** 
 * allocates memory for the level l containing M grids and
 * attaches allocated memory space to gset structure.
 * If such level exists or there are no l-1 level
 * the function prints a warning message and does not allocate
 * memory.
 * 
 * @param gset pointer to the h_gset structure
 * @param l level to be created
 * @param M number of grids on l-th level
 */
void h_alloc_add_glevel ( h_gset *gset, int l, int M )
{
  char *fnc_msg = "Adding glevel to gset";

  int m;
  
  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "cannot free unallocated struct",
                  WARNING, 0 );
  }
  else if ( l <= gset->L-1 ) {
      _STAT_MSG ( fnc_msg,
                  "glevel you wanted to allocate is currently allocated",
                  WARNING, 0 );
  }
  else if ( l > gset->L ) {
      _STAT_MSG ( fnc_msg,
                  "you can not allocate this glevel, "
                  "lower levels are not yet allocated",
                  WARNING, 0 );
  }
  else {
      gset->L = gset->L+1;
      gset->glevel = (h_glevel**) realloc ( gset->glevel, gset->L*sizeof( h_glevel* ) );

      gset->glevel[l] = (h_glevel*) malloc ( sizeof( h_glevel ) );
      gset->glevel[l]->l = l;
      gset->glevel[l]->M = M;

      gset->glevel[l]->grid = (h_grid**) malloc ( M*sizeof( h_grid* ) );

      for (m = 0; m < M; m++) {
          gset->glevel[l]->grid[m] = (h_grid*) malloc ( sizeof( h_grid ) );
      }
  }
}


/** 
 * allocates memory for the m-th grid at level l and
 * attaches allocated memory space to gset structure.
 * If such grid (with m-th id) exists or there are no l level
 * the function prints a warning message and does not allocate
 * memory.
 * 
 * @param gset pointer to the h_gset structure
 * @param l level at which the grid is to be create
 * @param m id of grid which has to be created
 */
void h_alloc_add_grid ( h_gset *gset, int l, int m )
{
  char *fnc_msg = "Adding grid to gset";

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "cannot free unallocated struct",
                  WARNING, 0 );
  }
  else if ( l > gset->L ) {
      _STAT_MSG ( fnc_msg,
                  "cannot allocate this grid, "
                  "lower levels are not yet allocated",
                  WARNING, 0 );
  }
  else if ( m != gset->glevel[l]->M  ) {
      _STAT_MSG ( fnc_msg,
                  "cannot allocate this grid, "
                  "lower grids allocated/not allocated",
                  WARNING, 0 );
  }
  else {
      gset->glevel[l]->M += 1;
      
      gset->glevel[l]->grid =
          (h_grid**) realloc ( gset->glevel[l]->grid, gset->glevel[l]->M*sizeof( h_grid* ) );
      
      gset->glevel[l]->grid[m] = (h_grid*) malloc ( sizeof( h_grid ) );
  }
}


/** 
 * h_free_gset frees the memory space pointed by gset
 * 
 * @param gset gset pointer to the h_gset structure
 */
void h_free_gset ( h_gset *gset )
{
  char *fnc_msg = "Freeing gset";

  int l, m;
  int L, M;
  
  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "cannot free unallocated struct",
                  WARNING, 0 );
  }
  else {
      L = gset->L; 
      for (l = 0; l < L; l++) {
          M = gset->glevel[l]->M;
          for (m = 0; m < M; m++) {
              free ( gset->glevel[l]->grid[m] );
          }
          free( gset->glevel[l]->grid );
          free( gset->glevel[l] );
      }
      free ( gset->glevel );
      free ( gset );
      
      gset = NULL;
  }
}


/** 
 * This function prints out to stdout information about
 * the structure h_gset
 * 
 * @param gset pointer to the h_gset structure
 */
void h_info_gset ( h_gset *gset )
{
  char *fnc_msg = "Info gset";

  int l;
  int L, M;

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "grid is unallocated",
                  WARNING, 0 );
  }
  else {
      L = gset->L; /* # of allocated levels in gset */

      printf(" * h_gset structure contains %d allocated levels:\n", L);

      for (l = 0; l < L; l++) {
          M = gset->glevel[l]->M;
          
          printf ("  ** level %d contains %d allocated grids"
                  " with identifiers %d...%d\n", l, M, 0, M-1);
      }
      printf("\n");
  }
}
