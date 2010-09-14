

#include "flag_test.h"

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


int main( void )
{
  int i, j;
  const int rank = 2;
  const int N = 11;
  
  const H_DBL xL = -1.;
  const H_DBL xR = 1.;

  h_grid * g_t;

  h_grid * g = h_alloc_grid ( );

  h_amrp * p = h_alloc_amrp ( );

  h_fnc * f = h_alloc_fnc ( NULL, rank, sin_, one_ );

  int *id_fp;
  int Nfp;

  int *idL, *idR, Ngrids;

  _fnc_1D fnc_ptr;

  fnc_ptr = (f->C_da[0]);
  
  VL(("main.c fnc_ptr[0](0,NULL)=%f, fnc_ptr[0](0,NULL)=%f\n", fnc_ptr(0,NULL), fnc_ptr(0,NULL)  ));

  fnc_ptr = (f->C_da[1]);

  VL(("main.c fnc_ptr[0](0,NULL)=%f, fnc_ptr[0](0,NULL)=%f\n", fnc_ptr(0,NULL), fnc_ptr(0,NULL)  ));
  
  
  p->rr = 2;
  p->buf = 1;
  p->sp = 2;
  p->lmax = 3;
  p->lmbd = 4;
  p->errt = 9.;

  h_init_coarse_grid ( g, xL, xR, N, rank );
  /* or equivalently
   * h_init_grid ( g, xL, xR, N, 0, 0, rank, 0, 0 );
   * master/coarsest grid has any ghost points and her
   * label in grids tree is (0,0)
   */


  h_flagging_points ( g, p, h_fc_Test, &id_fp, &Nfp );

  /* printf(" TEST main.c Nfp=%d\n", Nfp); */

  VL(( " Calling h_clustering_flagged\n"));
  h_clustering_flagged ( id_fp, Nfp, p->buf, g->N, &idL, &idR, &Ngrids );
  VL(( " Exiting h_clustering_flagged\n"));

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
  

  _h_assign_cauchy_data ( g, p, f );

  for (i = 0; i < g->N; i++) {
      VL(("main.c i=%d, x[i]=%f, u[i]=%f, u[i+N]=%f\n",
          i, g->x[i], g->u[i], g->u[i+N] ));
  }
  
  for (j = 0; j < Ngrids; j++) {
      g_t = g->children[j];
      VL(("children j=%d\n", j ));

      /* _h_acd_to_one_grid ( g_t, f ); */

      int N_t = g_t->N;
      
      for (i = 0; i < N_t+g_t->Lghost+g_t->Rghost; i++) {
          VL(("main.c i=%d, x[i]=%e, u[i]=%f, u[i+N]=%f\n",
          i, g_t->x[i], g_t->u[i], g_t->u[i+N_t+g_t->Lghost+g_t->Rghost] ));
      
      }
  }

  /* h_1D_plot_one_grid ( g, "test main grid", 3 ); */
  /* h_1D_plot_one_grid ( g->children[1], "test children grid", 3 ); */
  h_1D_plot_set_of_grids ( g, 1, "plot set of grids", -1 );

  gnuplot_ctrl * handler = gnuplot_init();

  g_t = g->children[1];

  gnuplot_plot_xy( handler, g_t->x, g_t->u, g->N+g->Lghost+g->Rghost, NULL );

  /* gnuplot_plot_xy( handler, h_get_grid_positions(g_t), g_t->u, g->N, NULL ); */

  gnuplot_plot_xy( handler, h_get_grid_positions(g_t), h_get_grid_values(g_t, 0), g->N, NULL );

  sleep (1);
  
  gnuplot_close( handler );

  H_DBL *x, *u, *udot;
  x= h_get_grid_positions(g_t);
  u= h_get_grid_values(g_t, 0);
  udot= h_get_grid_values(g_t, 1);

  printf("TEST\n\n");
  
  for (i = 0; i < g_t->N; i++) {
      VL(("main.c i=%d, x[i]=%e, u[i]=%f, udot[i]=%f\n",
          i, x[i], u[i], udot[i] ));
  }

  x= h_get_grid_positions_wghosts(g_t);
  u= h_get_grid_values_wghosts(g_t, 0);
  udot= h_get_grid_values_wghosts(g_t, 1);

  printf("TEST\n\n");
  
  for (i = 0; i < g_t->N+g_t->Lghost+g_t->Rghost; i++) {
      VL(("main.c i=%d, x[i]=%e, u[i]=%f, udot[i]=%f\n",
          i, x[i], u[i], udot[i] ));
  }

  h_1D_plot_one_grid ( g_t, 0, H_FALSE, "h_1D_plot_one_grid", -1 );
  h_1D_plot_one_grid ( g_t, 1, H_FALSE, "h_1D_plot_one_grid", -1 );

  h_1D_plot_set_of_grids ( g, 1, "h_1D_plot_set_of_grids", -1 );

  
  free ( idL );

  free ( idR );
  
  free( id_fp );

  h_free_fnc( f );

  h_free_grid ( g );
  
  h_free_amrp ( p );
  
  VL(( " main.c Exiting flag_test\n"));
  
  return 0;
}
