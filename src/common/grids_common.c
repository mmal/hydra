

#include "grids_common.h"



/** 
 * Allocates memory for a h_grid structure
 * except grids points and grids values
 * (respectively vectors x and u) and
 * sets all values to zero and NULL.
 * 
 * @return pointer to the h_grid structure
 */
h_grid * h_alloc_grid ( void )
{
  h_grid * g = (h_grid*) malloc ( sizeof( h_grid ) );

  if ( g == NULL )
      _STAT_MSG ( "Allocating grid",
                  "cannot allocate grid",
                  WARNING, 0 );
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
  }
  return g;
}



/** 
 * Initializes, previously allocated, h_grid structure
 * allocating memory space to grids points and grids values
 * (respectively vectors x and u) and calculates grids positions
 * (equally spaced).
 * 
 * @param g pointer to the h_grid structure which is to be initialized
 * @param xL position of the left boundary (physical one)
 * @param xR position of the right boundary (physical one)
 * @param N number of grid points
 * @param Lghost number of left ghost points
 * @param Rghost number of right ghost points
 * @param rank rank of the equation
 * @param l index of the level at which grid is located
 * @param m index of the grid at the level l
 */
void h_init_grid ( h_grid * g, H_DBL xL, H_DBL xR,
                   int N, int Lghost, int Rghost,
                   int rank, int l, int m )
{
  int i, Ntotal;

  char * fnc_msg = (char*) malloc( 35*sizeof(char) );

  sprintf( fnc_msg, "Initializing grid l=%d, m=%d", l, m );
  if ( g == NULL )
      _STAT_MSG ( fnc_msg,
                  "h_grid is unallocated",
                  WARNING, 0 );

  else if ( N<2  )
      _STAT_MSG ( fnc_msg,
                  "number of grid points must be >2",
                  ERROR, 0 );
  else if ( rank < 0 )
      _STAT_MSG ( fnc_msg,
                  "equation rank must be >0",
                  WARNING, 0 );
  else if ( xR<=xL )
      _STAT_MSG ( fnc_msg,
                  "for all grids xR must be greater than xL",
                  WARNING, 0 );
  else {
      g->rank = rank;
      g->xL = xL;
      g->xR = xR;
      g->h = (xR-xL)/(N-1);
      g->N = N;
      g->Lghost = Lghost;
      g->Rghost = Rghost;

      Ntotal = N+Lghost+Rghost; /* the total number of grid points */
      g->Ntotal = Ntotal;

      /* TODO:
       * doubled instructions? */
      g->l = l;
      g->m = m;
      
      g->t = 0.;
      g->tlast = 0.;
      g->Ncalls = 0;
      
      g->x = ( H_DBL *) malloc ( Ntotal*sizeof( H_DBL ) );
      
      if (g->x == NULL ) {
          /* free ( g ); */ /*TODO: why to free or not to free g? */
          _STAT_MSG ( fnc_msg,
                      "cannot allocate memory for grid points",
                      WARNING, 0 );
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
      
      g->u = ( H_DBL *) malloc ( rank*Ntotal*sizeof( H_DBL ) );
      
      if (g->u == NULL ) {
          free ( g->x );
          /* free ( g ); */ /*TODO: why to free or not to free g? */
          _STAT_MSG ( fnc_msg,
                      "cannot allocate memory for the grids data",
                      WARNING, 0 );
      }
      
      /* TODO:
       * what with the previous grids structure? */
      
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
}



/** 
 * Initializes master, previously allocated, h_grid structure
 * allocating memory space to grids points and grids values
 * (respectively vectors x and u) and calculates grids positions
 * (equally spaced).
 * 
 * @param g pointer to the h_grid structure which is to be initialized
 * @param xL position of the left boundary (physical one)
 * @param xR position of the right boundary (physical one)
 * @param N number of grid points
 * @param Lghost number of left ghost points
 * @param Rghost number of right ghost points
 * @param rank rank of the equation
 */
void h_init_master_grid ( h_grid * g, H_DBL xL, H_DBL xR,
                          int N, int rank )
{
  h_init_grid ( g, xL, xR, N, 0, 0, rank, 0, 0 );
  g->master = NULL;
  g->is_master = H_TRUE;
  g->Nchildren = 0;
}



/** 
 * Returns pointer to the grids positions of grid without
 * ghost points.
 * 
 * @param g pointer to a h_grid structure from which we get the grid points
 * 
 * @return pointer to the x vector without ghosts
 */
H_DBL *h_get_grid_positions ( h_grid *g )
{
  int Lghost = g->Lghost;

  return g->x+Lghost;
}



/** 
 * Returns pointer to the grids positions of grid with
 * ghost points.
 * 
 * @param g pointer to a h_grid structure from which we get the grid points
 * 
 * @return pointer to the x vector with ghosts
 */
H_DBL *h_get_grid_positions_wghosts ( h_grid *g )
{
  return g->x;
}



/** 
 * Returns pointer to the grids values of grid without
 * ghost points.
 * 
 * @param g pointer to a h_grid structure from which we get the grid values
 * @param rank rank of the values of which are to be returned
 * 
 * @return pointer to the u vector without ghosts
 */
H_DBL *h_get_grid_values ( h_grid *g, int rank )
{
  int Lghost = g->Lghost;
  int Ntotal = g->Ntotal;

  return g->u+rank*Ntotal+Lghost;
}



/** 
 * Returns pointer to the grids values of grid with
 * ghost points.
 * 
 * @param g pointer to a h_grid structure from which we get the grid values
 * @param rank rank of the values of which are to be returned
 * 
 * @return pointer to the u vector with ghosts
 */
H_DBL *h_get_grid_values_wghosts ( h_grid *g, int rank )
{ 
  int Ntotal = g->Ntotal;

  return g->u+rank*Ntotal;
}



/** 
 * Frees the memory space pointed by g
 * 
 * @param g pointer to the h_grid structure
 */
void h_free_grid ( h_grid * g )
{
  char *fnc_msg = "Feeing grid";
  
  if ( g != NULL ) {
      if ( g->x != NULL )
          free ( g->x );

      if ( g->u != NULL )
          free ( g->u );
    
      free( g );
      g = NULL;
  }
  else
      _STAT_MSG ( fnc_msg,
                  "h_grid is unallocated",
                  WARNING, 0 );
}

/* TODO:
 *  free h_free_rem_glevel
 *  free h_free_rem_grid
 *  change all h_grid mallocs to h_alloc_grid()
 *  change all free( gset->glevel[l]->grid ) to h_free_grid()
 */



/** 
 * Allocates memory for a h_gset structure containing
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
          gset->glevel[l]->grid[m] = h_alloc_grid ();
          gset->glevel[l]->grid[m]->m = m;
          gset->glevel[l]->grid[m]->l = l;
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

      /* allocating grids and assigning identifiers */
      for (m = 0; m < M; m++) {
          gset->glevel[l]->grid[m] = h_alloc_grid ();
          gset->glevel[l]->grid[m]->m = m;
          gset->glevel[l]->grid[m]->l = l;
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
      
      /* allocating grid and assigning identifiers */
      gset->glevel[l]->grid[m] = h_alloc_grid ();
      gset->glevel[l]->grid[m]->m = m;
      gset->glevel[l]->grid[m]->l = l;
  }
}



/** 
 * Allocates memory for N grids at level l and
 * attaches allocated memory space to gset structure.
 * Grids are attached in parallel to the existing grids
 * at a given level l, with appropriate identifiers (continued).
 * If level l does not exists a function allocates it and creates
 * N grids on it. If there is no enough space the function prints
 * a warning message and does not allocate memory.
 * 
 * @param gset pointer to the h_gset structure
 * @param l level at which the grids are to be create
 * @param N number of grids which has to be created
 */
void h_alloc_add_N_grids ( h_gset *gset, int l, int N )
{
  char *fnc_msg = "Adding grid to gset";

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "cannot free unallocated struct",
                  WARNING, 0 );
  }
  else if ( l > gset->L + 1) {
      _STAT_MSG ( fnc_msg,
                  "can not allocate these grids, "
                  "lower levels are not yet allocated",
                  WARNING, 0 );
  }
  else if ( m != gset->glevel[l]->M  ) {
      _STAT_MSG ( fnc_msg,
                  "cannot allocate these grids, "
                  "lower grids allocated/not allocated",
                  WARNING, 0 );
  }
  else {
      gset->glevel[l]->M += 1;
      
      gset->glevel[l]->grid =
          (h_grid**) realloc ( gset->glevel[l]->grid, gset->glevel[l]->M*sizeof( h_grid* ) );
      
      /* allocating grid and assigning identifiers */
      gset->glevel[l]->grid[m] = h_alloc_grid ();
      gset->glevel[l]->grid[m]->m = m;
      gset->glevel[l]->grid[m]->l = l;
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
              h_free_grid ( gset->glevel[l]->grid[m] );
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
                  "h_gset is unallocated",
                  WARNING, 0 );
  }
  else {
      L = gset->L; /* # of allocated levels in gset */

      printf(" * h_gset structure contains %d allocated levels:\n", L);

      for (l = 0; l < L; l++) {
          M = gset->glevel[l]->M;
          
          if ( M > 0 )
              printf ("  ** level %d contains %d allocated grids"
                      " with identifiers %d...%d\n", l, M, 0, M-1 );
          else
              printf ("  ** level %d contains %d allocated grids\n",
                      l, M );

      }
      printf("\n");
  }
}



/** 
 * This function prints out to stdout information about
 * the structure h_glevel
 * 
 * @param glevel pointer to the h_glevel structure
 */
void h_info_glevel ( h_glevel *glevel )
{
  char *fnc_msg = "Info glevel";

  int l;
  int M;

  if ( glevel == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_glevel is unallocated",
                  WARNING, 0 );
  }
  else {
      l = glevel->l;
      M = glevel->M; /* # of allocated grids in glevel */

      printf(" * h_glevel structure:\n");
      printf ("  ** level %d contains %d allocated grids"
                  " with identifiers %d...%d\n", l, M, 0, M-1);
      printf("\n");
  }
}



/** 
 * This function prints out to stdout information about
 * the structure h_grid
 * 
 * @param grid pointer to the h_grid structure
 */
void h_info_grid ( h_grid *grid )
{
  char *fnc_msg = "Info grid";

  int l, m;

  if ( grid == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_grid is unallocated",
                  WARNING, 0 );
  }
  else {
      l = grid->l; /* level of grid */
      m = grid->m; /* id of grid in this level */

      printf(" * h_grid structure\n");
      printf("  ** is at level %d and has identifier %d\n",
             l, m);
      if ( grid->x == NULL ) {
          printf("     and is uninitialized\n");
      }
      else
          printf("     and is initialized\n");
      printf("\n");
  }
}



/** 
 * Returns pointer to the l-th h_glevel structure from h_gset structure
 * 
 * @param gset pointer to the h_gset structure
 * @param l index of the level to which the pointer is returned
 * 
 * @return pointer to the h_glevel structure
 */
h_glevel *h_point_to_glevel ( h_gset *gset, int l )
{
  char *fnc_msg = (char *) malloc ( 35*sizeof ( char ) );
  sprintf( fnc_msg, "Pointing at the glevel l=%d", l );

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else if ( gset->glevel == NULL || l > gset->L - 1 ) {
      _STAT_MSG ( fnc_msg,
                  "such h_glevel is absent",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else if ( gset->glevel[l] == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "such h_glevel is unallocated",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else {
      free ( fnc_msg );
      return gset->glevel[l];
  }
}



/** 
 * Returns pointer to the m-th h_grid structure at the l-th level
 * from h_gset structure
 * 
 * @param gset pointer to the h_gset structure
 * @param l index of the level at which the grid is located
 * @param m index of the grid to which the pointer is returned
 * 
 * @return pointer to the h_grid structure
 */
h_grid *h_point_to_grid ( h_gset *gset, int l, int m )
{
  char *fnc_msg = (char *) malloc ( 35*sizeof ( char ) );
  sprintf( fnc_msg, "Pointing at the grid l=%d, m=%d", l, m );

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else if ( gset->glevel == NULL || l > gset->L - 1 ) {
      _STAT_MSG ( fnc_msg,
                  "such h_glevel is absent",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else if ( gset->glevel[l] == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "such h_glevel is unallocated",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else if ( m > gset->glevel[l]->M - 1 ) {
      _STAT_MSG ( fnc_msg,
                  "such h_grid in this h_glevel is absent",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else if ( gset->glevel[l]->grid[m] == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "such h_grid in this h_glevel is unallocated",
                  WARNING, 0 );
      free ( fnc_msg );
      return NULL;
  }
  else {
      free ( fnc_msg );
      return gset->glevel[l]->grid[m];
  }
}
