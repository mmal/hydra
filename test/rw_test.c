

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
  /* VL(("called RHS_centered\n")); */

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
  /* VL(("called RHS_extern_0\n")); */
  if ( i >= 0 ) {
      f[i] = 0.;
      f[N+i] = 0.;
  }
  else {
      f[N-1] = 0.;
      f[2*N-1] = 0.;
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
  /* VL(("called RHS_extern_1\n")); */
  f[abs(i)] = u[N+abs(i)];
  f[N+abs(i)] = fda_D2_5_extern_1_node( u, h, i );
  /* fda_D2_5_extern_1_node */
  return H_TRUE;
}



int main( int argc, char *argv[] )
{
  const int rank = 2;

  const int N = 21;
  
  const H_DBL xL = -1.0;

  const H_DBL xR = 1.0;


  
  h_hms *hms = h_alloc_hms( );

  h_hms *hms_cpy = h_alloc_hms( );

  
  /* h_create_run ( const char * path, const char *name, h_hms * hms );  */
  
  /* initialize utils */
  /* h_init_utils ( hms->utils ); */
  

  h_init_amrp ( hms->amrp, argc, argv );

  h_init_fnc_derivs ( hms->fnc, 3, RHS_extern_0, RHS_extern_1, RHS_centered );

  h_init_fnc_cauchy_data ( hms->fnc, NULL, rank, sin_, zero_ );

  h_init_fnc_step_type ( hms->fnc, gsl_odeiv_step_rk4 );

  h_init_fnc_flag_crit ( hms->fnc, h_fc_SV );

  h_init_master_grid ( hms->gset->glevel[0]->grid[0], xL, xR, N, rank, hms->amrp );

  h_create_init_gset ( hms );

  h_info_gset ( hms->gset );


  h_write_h5_amrp (  hms->utils->bas, "amrp_wt.h5", hms->amrp );
  
  h_read_h5_amrp (  hms_cpy->utils->bas, "amrp_wt.h5", hms_cpy->amrp );

  h_write_h5_amrp (  hms_cpy->utils->bas, "amrp_rwt.h5", hms_cpy->amrp );


  h_write_h5_gset (  hms->utils->bas, "gset_wt.h5", hms->gset );
  
  h_read_h5_gset (  hms_cpy->utils->bas, "gset_wt.h5", hms_cpy->gset );

  h_write_h5_gset (  hms_cpy->utils->bas, "gset_rwt.h5", hms_cpy->gset );


  

  h_free_hms ( hms_cpy );

  h_free_hms ( hms );
    
  return 0;
}
