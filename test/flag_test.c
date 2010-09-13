

#include "flag_test.h"

H_DBL zero_ ( H_DBL x, void *params )
{
  void *newp;
  newp = (void*) params;
  x = 0.;
  return 0.1;
}

H_DBL one_ ( H_DBL x, void *params )
{
  void *newp;
  newp = (void*) params;
  x = 0.;
  return 1.;
}

int main( void )
{
  int i;
  const int rank = 2;
  const int N = 11;
  
  const H_DBL xL = -1.;
  const H_DBL xR = 1.;

  h_grid * g_t;

  h_grid * g = h_alloc_grid ( );

  h_amrp * p = h_alloc_amrp ( );

  h_fnc * f = h_alloc_fnc ( NULL, rank, zero_, one_ );

  int *id_fp;
  int Nfp;

  int *idL, *idR, Ngrids;

  /* H_DBL (*fnc_ptr)( H_DBL, void * ); */

  _fnc_1D fnc_ptr;

  fnc_ptr = (f->C_da[0]);
  
  VL(("main.c fnc_ptr[0](0,NULL)=%f, fnc_ptr[0](0,NULL)=%f\n", fnc_ptr(0,NULL), fnc_ptr(0,NULL)  ));

  fnc_ptr = (f->C_da[1]);

  VL(("main.c fnc_ptr[0](0,NULL)=%f, fnc_ptr[0](0,NULL)=%f\n", fnc_ptr(0,NULL), fnc_ptr(0,NULL)  ));
  
  h_free_fnc( f );
  
  p->rr = 2;
  p->buf = 1;
  p->sp = 2;
  p->lmax = 3;
  p->lmbd = 4;
  p->errt = 9.;

  h_init_coarse_grid ( g, xL, xR, N, rank );
  /* or equivalently */
  /* h_init_grid ( g, xL, xR, N, 0, 0, rank, 0, 0 ); */

  /* printf("TEST\n"); */

  h_flagging_points ( g, p, h_fc_Test, &id_fp, &Nfp );

  /* printf(" TEST main.c Nfp=%d\n", Nfp); */

  L( " Calling h_clustering_flagged\n");
  h_clustering_flagged ( id_fp, Nfp, p->buf, g->N, &idL, &idR, &Ngrids );
  L( " Exiting h_clustering_flagged\n");

  /* i=1; */
  /* if( id_fp == NULL ) printf( "TEST main.c id_fp == NULL\n" ); */

  /* /\* printf(" TEST main.c if_fp[%d]=%d\n", i, id_fp[i]); *\/ */

  for (i = 0; i < Nfp; i++) {
      printf("main.c i=%d, id_fp=%d\n", i, id_fp[i]);
  }

  for (i = 0; i < Ngrids; i++) {
      printf("main.c i=%d, idL=%d, idR=%d\n", i, idL[i], idR[i]);
  }

  _h_create_offspring_grids ( g, p, idL, idR, Ngrids );

  for (i = 0; i < Ngrids; i++) {
      g_t = g->children[i];
      printf("main.c i=%d, xL=%f, xR=%f\n", i, g_t->xL, g_t->xR );
      printf("main.c i=%d, N=%d, h=%f\n", i, g_t->N, g_t->h );
  }
  
  free ( idL );

  free ( idR );
  
  free( id_fp );

  h_free_grid ( g );
  
  h_free_amrp ( p );
  
  VL(( " main.c Exiting flag_test\n"));
  
  return 0;
}
