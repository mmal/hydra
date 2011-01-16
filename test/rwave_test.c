

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


H_DBL gauss_x_ ( H_DBL x, void *params )
{
  void *newp = (void*) params;

  H_DBL exponent = (x-3.0)*(x-3.0);

  if ( exponent > 40.0 )
      return 0.0;
  else  
      return (x-1.0)*exp(-exponent);
}


int RHS_centered ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams )
{
  H_DBL h = *(H_DBL *) vparams;
  H_DBL eps = 0.01/h;
  
  t=x[0];
  t=u[0];
  t=h;
  /* VL(("called RHS_centered\n")); */

  f[abs(i)] = u[N+abs(i)];

  if ( i >=3 && i<= N-3 )
      f[N+abs(i)] = fda_D2_5_inner_node ( u, h, i ) + 2.0/x[abs(i)]*fda_D1_5_inner_node ( u, h, i ) + eps*(u[-3+i]-6*u[-2+i]+15*u[-1+i]-20*u[i]+15*u[1+i]-6*u[2+i]+u[3+i]);
  else if ( i == 2 )
      f[N+abs(i)] = fda_D2_5_inner_node ( u, h, i ) + 2.0/x[abs(i)]*fda_D1_5_inner_node ( u, h, i ) + eps*(2*u[sgn(i)*(-2+i)]-13*u[sgn(i)*(-1+i)]+36*u[sgn(i)*(i)]-55*u[sgn(i)*(1+i)]+50*u[sgn(i)*(2+i)]-27*u[sgn(i)*(3+i)]+8*u[sgn(i)*(4+i)]-u[sgn(i)*(5+i)]);
  else if ( i == N-2 ) {
      i = -i;
      f[N+abs(i)] = fda_D2_5_inner_node ( u, h, i ) + 2.0/x[abs(i)]*fda_D1_5_inner_node ( u, h, i ) + eps*(2*u[sgn(i)*(-2+i)]-13*u[sgn(i)*(-1+i)]+36*u[sgn(i)*(i)]-55*u[sgn(i)*(1+i)]+50*u[sgn(i)*(2+i)]-27*u[sgn(i)*(3+i)]+8*u[sgn(i)*(4+i)]-u[sgn(i)*(5+i)]);
  }
  else
      f[N+abs(i)] = fda_D2_5_inner_node ( u, h, i ) + 2.0/x[abs(i)]*fda_D1_5_inner_node ( u, h, i );

    /* f[N+abs(i)] = fda_D2_5_inner_node ( u, h, i ) + 2.0/x[i]*fda_D1_5_inner_node ( u, h, i ); */
    /* f[N+abs(i)] = fda_D2_3_inner_node( u, h, abs(i) ) + 2.0/x[abs(i)]*fda_D1_3_inner_node ( u, h, abs(i) ); */

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
      f[i] = 0.0;
      f[N+i] = 0.0;
  }
  else {
      f[N-1] = 0.0;
      f[2*N-1] = 0.0;
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
  f[N+abs(i)] = fda_D2_5_extern_1_node( u, h, i ) + 2.0/x[abs(i)]*fda_D1_5_extern_1_node ( u, h, i );
  /* f[N+abs(i)] = fda_D2_3_inner_node( u, h, abs(i) ) + 2.0/x[abs(i)]*fda_D1_3_inner_node ( u, h, abs(i) ); */

  /* fda_D2_5_extern_1_node */
  return H_TRUE;
}


int main( int argc, char *argv[] )
{
  const int rank = 2;

  const int N = 31;
  
  const H_DBL xL = 1.0;

  const H_DBL xR = 15.0;

  const H_DBL T = 15.e0;
  
  h_hms *hms = h_alloc_hms( );

  h_hms *hms_cpy = h_alloc_hms( );

  
  /* h_create_run ( const char * path, const char *name, h_hms * hms );  */
  
  /* initialize utils */
  /* h_init_utils ( hms->utils ); */
  

  h_init_amrp ( hms->amrp, argc, argv );

  h_init_fnc_derivs ( hms->fnc, 3, RHS_extern_0, RHS_extern_1, RHS_centered );

  h_init_fnc_cauchy_data ( hms->fnc, NULL, rank, gauss_x_, zero_ );

  h_init_fnc_step_type ( hms->fnc, gsl_odeiv_step_rk4 );

  h_init_fnc_flag_crit ( hms->fnc, h_fc_SV );

  h_init_master_grid ( hms->gset->glevel[0]->grid[0], xL, xR, N, rank, hms->amrp );

  h_create_init_gset ( hms );

  h_info_gset ( hms->gset );




  
  
  while ( hms->gset->glevel[0]->grid[0]->t < T ) {
      printf( "t=%e\n", hms->gset->glevel[0]->grid[0]->t );
      h_boialg ( hms );
      h_1Dplot_save_gset ( hms->gset, 0, H_FALSE, "gset: rank 0", -1 );
      /* h_1Dplot_save_gset ( hms->gset, 1, H_TRUE, "gset: rank 1", 0 ); */
  }

  /* int status; */
  /* while ( hms->gset->glevel[0]->grid[0]->t < T ) { */
  /*     status = _h_boialg_step_glevel ( hms->gset->glevel[0], hms->amrp, hms->fnc ); */

  /*     status = _h_boialg_step_glevel ( hms->gset->glevel[1], hms->amrp, hms->fnc ); */

  /*     status = _h_boialg_step_glevel ( hms->gset->glevel[1], hms->amrp, hms->fnc ); */


  /*     /\* ten update nie działa ! *\/ */
  /*     status = _h_update_glevel ( hms->gset->glevel[0], hms->gset->glevel[1], hms->amrp ); */
      
  /*     /\* status = _h_update_grid_interior ( hms->gset->glevel[0]->grid[0], hms->gset->glevel[1]->grid[0], hms->amrp ); *\/ */

  /*     /\* status = _h_update_grid_ghosts_new ( hms->gset->glevel[0]->grid[0], hms->gset->glevel[1]->grid[0], hms->amrp ); *\/ */

  /*     /\* h_1Dplot_save_gset ( hms->gset, 0, H_TRUE, "gset: rank 0", 0 ); *\/ */
  /*     h_1Dplot_save_gset ( hms->gset, 1, H_TRUE, "gset: rank 1", 0 ); */
  /* } */



  
  /* h_1Dplot_save_grid ( h_point_to_grid( hms->gset, 0, 0 ), 0, H_TRUE, "grid 0,0: rank 0 ", -1 ); */
  /* h_1Dplot_save_grid ( h_point_to_grid( hms->gset, 0, 0 ), 1, H_TRUE, "grid 0,0: rank 1 ", -1 ); */

  /* h_1Dplot_save_gset ( hms->gset, 0, H_FALSE, "gset: rank 0", -1 ); */
  /* h_1Dplot_save_gset ( hms->gset, 1, H_FALSE, "gset: rank 1", -1 ); */


  /* H_DBL *xnear; */
  /* int j; */
  /* xnear = _h_find_5_nearest ( -0.90, h_point_to_grid( hms->gset, 0, 0 ), hms->amrp ); */
  /* for (j = 0; j < 5; j++) { */
  /*     printf("xnear[j=%d]=%e\n", j, xnear[j] ); */
  /* } */

  /* free( xnear ); */

  /* { */
  /*     int *inear; */
  /*     int j; */
  /*     H_DBL *xgrid = h_get_grid_positions_wghosts ( h_point_to_grid( hms->gset, 0, 0 ) ); */
  /*     H_DBL *ugrid = h_get_grid_values_wghosts ( h_point_to_grid( hms->gset, 0, 0 ), 0 ); */
  /*     H_DBL *udotgrid = h_get_grid_values_wghosts ( h_point_to_grid( hms->gset, 0, 0 ), 1 ); */
  /*     inear = _h_find_5_nearest_indices ( 1.180, h_point_to_grid( hms->gset, 0, 0 ), hms->amrp ); */
  /*     for (j = 0; j < 5; j++) { */
  /*         printf("inear[j=%d]=%d x[j]=%e u[j]=%e udot[j]=%e\n", */
  /*                j, inear[j], xgrid[inear[j]], ugrid[inear[j]], udotgrid[inear[j]] ); */
  /*     } */
  /*     free( inear ); */
  /* } */

  
  h_free_hms ( hms_cpy );

  h_free_hms ( hms );
    
  return 0;
}
