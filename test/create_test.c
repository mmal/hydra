

#include "create_test.h"


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

  f[i] = u[N+i];
  f[N+i] = fda_D2_5_inner_node ( u, h, i );

  return H_TRUE;
}


int RHS_extern_0 ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams )
{
  H_DBL h = *(H_DBL *) vparams;

  t=x[0];
  t=u[0];
  t=h;
  
  f[i] = 0.;
  f[N+i] = 0.;

  return H_TRUE;
}


int RHS_extern_1 ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams )
{
  H_DBL h = *(H_DBL *) vparams;

  t=x[0];
  t=u[0];
  t=h;

  f[i] = u[N+i];
  f[N+i] = fda_D2_5_extern_1_node ( u, h, i );

  return H_TRUE;
}


int main( int argc, char *argv[] )
{
  const int rank = 2;

  const int N = 21;
  
  const H_DBL xL = -1.;

  const H_DBL xR = 1.;
  
  h_hms *m = h_alloc_hms ();
  
  h_init_fnc_derivs ( m->f, 3, RHS_extern_0, RHS_extern_1, RHS_centered );

  h_init_fnc_cauchy_data ( m->f, NULL, rank, sin_, zero_ );

  h_init_fnc_step_type ( m->f, gsl_odeiv_step_rk4 );

  h_init_fnc_flag_crit ( m->f, &h_fc_Richardson );
  
  h_init_amrp ( m->p, argc, argv );


  /* m->p->ngh = (m->p->sp)*(m->p->rr)*4; */
  
  /* printf("m->f->C_da[0](0.25, NULL) = %e\n", (*m->f->C_da[0])(0.25, NULL)); */

  /* h_hms *m_c = h_alloc_hms(); */

  /* m_c->f=m->f; */
  
  /* printf("m_c->f->C_da[0](0.25, NULL) = %e\n", (*m_c->f->C_da[0])(0.25, NULL)); */


  h_init_coarse_grid ( m->g, xL, xR, N, rank );

  _h_create_set_of_grids ( m );

  printf("main: l=%d, m=%d\n\n", m->g->l, m->g->l );

  /* /\* m->g= (h_grid*) m->g->master; *\/ */
  /* /\* h_grid *gc = m->g; *\/ */
  
  h_1D_plot_set_of_grids_2 ( m->g, 0, H_FALSE, "h_1D_plot_set_of_grids", -1 );

  printf ("m->g->is_master = %d\n", m->g->is_master );
  
  h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "h_1D_plot_set_of_grids", -1 );


  /* printf( "   main: integrating\n\n\n"); */
  /* _h_fc_integrate ( 0., 0.01, m->g->h/4., m->g->u, m ); */
  
  /* m->g->t=1.; */
  
  /* h_1D_plot_set_of_grids_2 ( m->g, 0, H_TRUE, "h_1D_plot_set_of_grids", -1 ); */

  /* h_free_hms ( m ); */

  VL(( "create_test.c Exiting\n"));
  
  return 0;
}

