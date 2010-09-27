#include "h_grid_alloc_test.h"


int main( void )
{
  h_grid * g = h_alloc_grid ( );

  h_init_coarse_grid ( g, -1., 1., 11, 1 );

  h_free_grid ( g );

  return 0;
}
