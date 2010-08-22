#include "h_test.h"

int main(int argc, char *argv[])
{
  int i;
  i = 0;
  
  h_grid * g = h_grid_alloc ( );

  free( g );
  return 0;
}
