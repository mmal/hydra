#include "h_integr_test.h"


int func (H_DBL t, const H_DBL y[], H_DBL f[],
          void *params)
{
  H_DBL mu = *(H_DBL *)params;
  f[0] = y[1];
  f[1] = -y[0] - mu*y[1]*(y[0]*y[0] - 1);
  t = 0.0;
  return GSL_SUCCESS;
}
     
int jac (H_DBL t, const H_DBL y[], H_DBL *dfdy, 
     H_DBL dfdt[], void *params)
{
  H_DBL mu = *(H_DBL *)params;
  gsl_matrix_view dfdy_mat 
      = gsl_matrix_view_array (dfdy, 2, 2);
  gsl_matrix * m = &dfdy_mat.matrix; 
  gsl_matrix_set (m, 0, 0, 0.0);
  gsl_matrix_set (m, 0, 1, 1.0);
  gsl_matrix_set (m, 1, 0, -2.0*mu*y[0]*y[1] - 1.0);
  gsl_matrix_set (m, 1, 1, -mu*(y[0]*y[0] - 1.0));
  dfdt[0] = 0.0;
  dfdt[1] = 0.0;
  t = 0.0;
  return GSL_SUCCESS;
}



int main (void)
{
  const gsl_odeiv_step_type * T 
      = gsl_odeiv_step_rk4;
  
  gsl_odeiv_step * s 
      = gsl_odeiv_step_alloc (T, 2);
     
  H_DBL mu = 10;

  gsl_odeiv_system sys = {func, jac, 2, &mu};
     
  H_DBL t = 0.0, t1 = 10.0;
  H_DBL h = 1e-2;
  H_DBL y[2] = { 1.0, 0.0 }, y_err[2];
  H_DBL dydt_in[2], dydt_out[2];
     
  /* initialise dydt_in from system parameters */
  GSL_ODEIV_FN_EVAL(&sys, t, y, dydt_in);
     
  while (t < t1)
    {
        int status = gsl_odeiv_step_apply (s, t, h, 
                                           y, y_err, 
                                           dydt_in, 
                                           dydt_out, 
                                           &sys);
     
        if (status != GSL_SUCCESS)
            break;
     
        dydt_in[0] = dydt_out[0];
        dydt_in[1] = dydt_out[1];
     
        t += h;
        
        printf ("%.5e %.5e %.5e\n", t, y[0], y[1]);
    }
     
  gsl_odeiv_step_free (s);
  return 0;
}
