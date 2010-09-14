

#include "create_test.h"

H_DBL zero_ ( H_DBL x, void *params )
{
  void *newp;
  newp = (void*) params;
  x = 0.;
  return 0.031415;
}

H_DBL one_ ( H_DBL x, void *params )
{
  void *newp;
  newp = (void*) params;
  x = 0.;
  return 1.;
}

H_DBL sin_ ( H_DBL x, void *params )
{
  void *newp;
  newp = (void*) params;
  return sin(2*M_PI*x);
}


int main ( void )
{
  const int rank = 2;

  const int N = 11;
  
  const H_DBL xL = -1.;

  const H_DBL xR = 1.;
  
  h_grid * g = h_alloc_grid ( );

  h_amrp * p = h_alloc_amrp ( );

  h_fnc * f = h_alloc_fnc ( NULL, rank, sin_, one_ );

  p->rr = 2;
  p->buf = 1;
  p->sp = 2;
  p->lmax = 3;
  p->lmbd = 4;
  p->errt = 9.;

  h_init_coarse_grid ( g, xL, xR, N, rank );

  _h_create_set_of_grids ( g, p, f );

  h_1D_plot_set_of_grids ( g, 0, "h_1D_plot_set_of_grids", -1 );
  
  h_free_fnc( f );
  h_free_grid ( g );
  h_free_amrp ( p );

  VL(( "create_test.c Exiting\n"));
  
  return 0;
}


/* { */
/*   h_init_coarse_grid ( g, xL, xR, N, rank ); */

/*   h_flagging_points ( g, p, h_fc_Test, &id_fp, &Nfp ); */

/*   h_clustering_flagged ( id_fp, Nfp, p->buf, g->N, &idL, &idR, &Ngrids ); */

/*   _h_create_offspring_grids ( g, p, idL, idR, Ngrids ); */

/*   _h_assign_cauchy_data ( g, p, f ); */

/*   h_1D_plot_set_of_grids ( g, 1, "h_1D_plot_set_of_grids", -1 ); */


  
/*   h_free_fnc( f ); */
/*   h_free_grid ( g ); */
/*   h_free_amrp ( p ); */
  
/*   VL(( "create_test.c Exiting\n")); */
  
/*   return 0; */


/* } */
