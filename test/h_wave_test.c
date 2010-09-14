#include "h_wave_test.h"


int func_wave (H_DBL t, const H_DBL y[], H_DBL f[], void *params)
{
  int i;
  
  h_grid * g = ( h_grid *) params;

  const int N = g->N;
  const H_DBL h = g->h;

  H_DBL *yptr = y;
  
  for (i = 1; i < N-1; ++i) {
      f[i] = y[N+i];
      f[N+i] = fda_D2_eon_3 (yptr, h, N, i ); 
  }
  f[0] = 0.;
  f[N-1] = 0.;

  t = 1;
  
  return GSL_SUCCESS;
}


int jac_wave (H_DBL t, const H_DBL y[], H_DBL *dfdy, 
              H_DBL dfdt[], void *params)
{
  H_DBL temp;
  void *vtemp;

  temp = t;
  temp = dfdy[0];
  temp = dfdt[0];
  temp = y[0];
  vtemp = params;
  
  printf("This stepper requires jacobian matrix\n"); 
  return 0;
}


H_DBL sin_ (H_DBL x, void * params)
{
  h_grid * g = (h_grid *) params;

  H_DBL xL = g->xL;
  H_DBL xR = g->xR;

  return sin(2*M_PI*x/(xR-xL));
}


H_DBL zero_ (H_DBL x, void * params)
{
  void *temp;
  temp = params;
  x = 0;
  return 0.;
}


int main (void)
{
  FILE *fp = fopen("file", "w");

  int i, status;
  const int ord = 2;
  const int N = 11;
  
  const H_DBL xL = -1.;
  const H_DBL xR = 1.;
  
  h_grid * g = h_alloc_grid ( );

  h_amrp * p = h_alloc_amrp ( );
  
  const gsl_odeiv_step_type * T
      = gsl_odeiv_step_rk4;
     
  gsl_odeiv_step * s
      = gsl_odeiv_step_alloc (T, ord*N);
     
  gsl_odeiv_system sys = {func_wave, jac_wave, ord*N, g};
     
  H_DBL t = 0.0, t1 = 1.;
  H_DBL h = 1e-1;

  H_DBL * dydt_in  = ( H_DBL* ) malloc( ord*N*sizeof( H_DBL ) );
  H_DBL * dydt_out = ( H_DBL* ) malloc( ord*N*sizeof( H_DBL ) );
  H_DBL * y_err = ( H_DBL* ) malloc( ord*N*sizeof( H_DBL ) );


  p->rr = 2;
  p->buf = 2;
  p->sp = 2;
  p->lmax = 3;
  p->lmbd = 4;
  
  h_init_coarse_grid ( g, xL, xR, N, ord );

  h_ivp_1D ( g, g, ord, sin_, zero_ );  

  h_gen_grid_family ( g, p );

  h_ivp_1D_grid_family ( g, p, g, ord, sin_, zero_ );

  /* initialise dydt_in from system parameters */
  GSL_ODEIV_FN_EVAL(&sys, t, g->u, dydt_in);
     
  while (t < t1)
    {
        
        for (i = 0; i < N; i++) {
            fprintf (fp, "%.5e %.5e %.5e %.5e\n", t, g->x[i], g->u[i], g->u[N+i]);
        }
        fprintf(fp, "\n\n");
        
        h_1D_plot_one_grid ( g, 0, H_FALSE, NULL, 2 );

        h_1D_plot_set_of_grids ( g, p->lmax, NULL, 10 );
        
        status = gsl_odeiv_step_apply (s, t, h,
                                           g->u, y_err,
                                           dydt_in,
                                           dydt_out,
                                           &sys);
        
        if (status != GSL_SUCCESS)
            break;

        memcpy(dydt_in,dydt_out,(ord*N)*sizeof(H_DBL));
     
        t += h;
        g->t = t;
    }

  gsl_odeiv_step_free (s);

  h_free_grid ( g );

  h_free_amrp ( p );
  
  fclose( fp );
  
  return 0;
}
