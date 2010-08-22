#include "h_grids_common.h"

h_grid * h_grid_alloc ( void )
{
  h_grid * g = (h_grid*) malloc ( sizeof( h_grid ) );

  if ( g == NULL ) printf ( " g == NULL \n");
  else  printf ( " g != NULL \n");
  
  if ( g->offspring == NULL ) printf ( " offspring == NULL \n");
  else  printf ( " offspring != NULL \n");

  printf ( " h_grid alloc \n");
  
  return g;
}
