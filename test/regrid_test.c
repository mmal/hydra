

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

  H_DBL exponent = (x-2.0)*(x-2.0);

  if ( exponent > 30.0 )
      return 0.0;
  else  
      return (x)*exp(-exponent);
}


int RHS_centered ( H_DBL t, H_DBL *x, H_DBL *u, H_DBL *f,
                   int i, int N, void *vparams )
{
  H_DBL h = *(H_DBL *) vparams;
  H_DBL eps = 0.0;
  
  /* t=x[0]; */
  /* t=u[0]; */
  /* t=h; */
  /* VL(("called RHS_centered\n")); */

  f[i] = u[N+i];
  /* f[N+i] = ( u[abs(i)+1]-2*u[abs(i)]+u[abs(i)-1] )/( h*h ); */
  f[N+i]=(-u[i-2]+16*u[i-1]-30*u[i]+16*u[i+1]-u[i+2])/(12*h*h);

  /* fda_D2_3_inner_node ( u, h, i ); */

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
  /* if ( i == 0 ) { */
  /*     f[i] = 0.0; */
  /*     f[N+i] = 0.0; */
  /* } */
  /* else { */
  /*     f[N-1] = 0.0; */
  /*     f[2*N-1] = 0.0; */
  /* } */

  /* i=0 */
  f[0] = 0.0;
  f[N] = 0.0;

  /* i=N-1 */
  f[N-1] = 0.0;
  f[2*N-1] = 0.0;
  
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
  f[N+abs(i)] = ( u[abs(i)+1]-2*u[abs(i)]+u[abs(i)-1] )/( h*h );
  /* f[N+abs(i)] = (11*u[sgn(i)*(i-1)]-20*u[sgn(i)*i]+6*u[sgn(i)*(i+1)]+4*u[sgn(i)*(i+2)]- */
  /*                u[sgn(i)*(i+3)])/(12*h*h); */


  /* fda_D2_3_inner_node( u, h, fabs(i) ) */
      /* fda_D2_5_extern_1_node( u, h, i ) */;
  /* fda_D2_5_extern_1_node */
  return H_TRUE;
}



/**
 * The main function
 * 
 */
int main( int argc, char *argv[] )
{
  const int rank = 2;

  const int N = 201;
  
  const H_DBL xL = 0.0;

  const H_DBL xR = 25.0;

  const H_DBL T = 25.e0;

  int Nstep = 0;

  h_hms *hms = h_alloc_hms( );
  

  h_init_amrp ( hms->amrp, argc, argv );

  h_init_fnc_derivs ( hms->fnc, 3, RHS_extern_0, RHS_extern_1, RHS_centered );

  h_init_fnc_cauchy_data ( hms->fnc, NULL, rank, gauss_x_, zero_ );

  h_init_fnc_step_type ( hms->fnc, gsl_odeiv_step_rk4 );

  h_init_fnc_flag_crit ( hms->fnc, h_fc_SV );

  h_init_master_grid ( hms->gset->glevel[0]->grid[0], xL, xR, N, rank, hms->amrp );

  h_create_init_gset ( hms );

  h_info_gset ( hms->gset );
 
 
 
  FILE *fp = fopen("file", "w");
  
  while ( hms->gset->glevel[0]->grid[0]->t < T ) {

      printf( "t=%e\n", hms->gset->glevel[0]->grid[0]->t );

      h_boialg ( hms );

      Nstep++;

      if (Nstep%10==0)
        {
            h_regrid ( hms );
        }

      h_1Dplot_save_gset ( hms->gset, 0, H_FALSE, "gset: rank 0", -1 );

      fprintf(fp, "%e %e %e\n", hms->gset->glevel[0]->grid[0]->t, hms->gset->glevel[0]->grid[0]->u[1], hms->gset->glevel[0]->grid[0]->u[N+1]);
      
      /* h_1Dplot_save_gset ( hms->gset, 1, H_TRUE, "gset: rank 1", 0 ); */
  }

  fclose( fp );

  h_free_hms ( hms );
    
  return 0;
}
