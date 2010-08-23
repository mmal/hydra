#include "h_test.h"

int main( void )
{
  h_grid * g = h_grid_alloc ( );

  free( g );
  return 0;
}
