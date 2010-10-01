

#include "set_test.h"


H_DBL zero_ ( H_DBL x, void *params )
{
  void *newp;
  newp = (void*) params;
  x = 0.;
  return 0.0;
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
  return sin(2.*M_PI*x);
}


int RHS_centered ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams )
{
  H_DBL h = *(H_DBL *) vparams;

  t=x[0];
  t=u[0];
  t=h;
  
  f[abs(i)] = u[N+abs(i)];
  f[N+abs(i)] = fda_D2_5_inner_node ( u, h, i );

  return H_TRUE;
}


int RHS_extern_0 ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams )
{
  H_DBL h = *(H_DBL *) vparams;

  t=x[0];
  t=u[0];
  t=h;

  if ( i >= 0 ) {
      f[i] = 0.;
      f[N+i] = 0.;
  }
  else {
      f[N-1] = 0.;
      f[2*N-1] = 0;
  }
  
  return H_TRUE;
}


int RHS_extern_1 ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams )
{
  H_DBL h = *(H_DBL *) vparams;

  t=x[0];
  t=u[0];
  t=h;
  
  f[abs(i)] = u[N+abs(i)];
  f[N+abs(i)] = fda_D2_3_inner_node ( u, h, abs(i) );
  /* fda_D2_5_extern_1_node */
  return H_TRUE;
}


int main( int argc, char *argv[] )
{
  const int rank = 2;

  const int N = 121;
  
  const H_DBL xL = -1.;

  const H_DBL xR = 1.;
  
  h_hms *hms = h_alloc_hms( );

  h_init_amrp ( hms->amrp, argc, argv );


  h_init_fnc_derivs ( hms->fnc, 3, RHS_extern_0, RHS_extern_1, RHS_centered );

  h_init_fnc_cauchy_data ( hms->fnc, NULL, rank, sin_, zero_ );

  h_init_fnc_step_type ( hms->fnc, gsl_odeiv_step_rk4 );

  h_init_fnc_flag_crit ( hms->fnc, h_fc_Test );

  h_init_master_grid ( hms->gset->glevel[0]->grid[0], xL, xR, N, rank );

  h_create_init_gset ( hms );

  h_info_gset ( hms->gset );

  h_1D_plot_one_grid ( h_point_to_grid( hms->gset, 1, 0 ), 0, H_FALSE, "asdasdf", -1 );

  h_1D_plot_one_grid ( h_point_to_master_grid( hms->gset ), 0, H_FALSE, "asdasdf", -1 );

  h_free_hms ( hms );
  
  /* { */
  /*     h_gset *gset = h_alloc_gset( ); */
      
  /*     h_info_gset ( gset ); */
      
  /*     h_alloc_add_glevel( gset, 1, 0); */

  /*     h_info_gset ( gset ); */
      
  /*     h_alloc_add_grid( gset, 1, 0); */

  /*     h_alloc_add_glevel( gset, 2, 16); */
      
  /*     h_info_gset ( gset ); */
      
  /*     h_glevel *glevel = h_point_to_glevel ( gset, 2 ); */

  /*     h_info_glevel ( glevel ); */

  /*     h_alloc_add_N_grids ( gset, 2, 30 ); */
      
  /*     h_info_glevel ( glevel ); */

  /*     h_info_gset ( gset ); */

  /*     h_grid *grid = h_point_to_grid ( gset, 0, 0 ); */

  /*     h_init_master_grid ( gset->glevel[0]->grid[0], xL, xR, N, rank ); */
        
  /*     h_info_grid ( grid ); */

  /*     h_free_gset ( gset ); */
  /* } */

  
  /* h_init_coarse_grid ( m->g, xL, xR, N, rank ); */

  /* _h_create_set_of_grids ( m ); */

  /* /\* printf("main: l=%d, m=%d\n\n", m->g->l, m->g->l ); *\/ */

  /* /\* /\\* m->g= (h_grid*) m->g->master; *\\/ *\/ */
  /* /\* /\\* h_grid *gc = m->g; *\\/ *\/ */
  
  /* /\* h_1D_plot_set_of_grids_2 ( m->g, 0, H_FALSE, "h_1D_plot_set_of_grids", -1 ); *\/ */

  /* /\* printf ("m->g->is_master = %d\n", m->g->is_master ); *\/ */
  
  /* h_1D_plot_set_of_grids_2 ( m->g, 0, H_FALSE, "created set of grids no gh", -1 ); */
  /* /\* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "set of grids with gh", -1 ); *\/ */

  /* /\* h_1D_plot_set_of_grids_2 ( m->g, 1, H_FALSE, "h_1D_plot_set_of_grids", -1 ); *\/ */


  /* /\* printf( "   main: integrating\n\n\n"); *\/ */
  /* /\* _h_fc_integrate ( 0., (m->g->dt)*20, m->g->dt, m->g->u, m ); *\/ */
  
  
  /* /\* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "main integrated rank 0", -1 ); *\/ */
  /* /\* h_1D_plot_set_of_grids_2 ( m->g, 1, H_TRUE, "main integrated rank 1", -1 ); *\/ */

  /* /\* printf("m->g->Ncalls = %d\n", m->g->Ncalls); *\/ */


  /* h_boialg ( m ); */

  /* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "after bioalg with gh", -1 ); */

  /* h_1D_plot_set_of_grids_2 ( m->g, 0, H_FALSE, "after bioalg no gh", -1 ); */

  /* h_1D_plot_set_of_grids_2 ( m->g, 1, H_FALSE, "after bioalg deriv no gh", -1 ); */

  /* h_1D_plot_set_of_grids_2 ( m->g, 1, H_TRUE, "after bioalg deriv with gh", -1 ); */

  
  /* h_free_hms ( m ); */

  /* VL(( "create_test.c Exiting\n")); */
  
  return 0;
}
