#include "derivs.h"

/**
 *
 * First order derivs
 *
 */
H_DBL fda_D1_3_inner_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 3-point scheme, inner node, order O(h^2) */
  return ( u[i+1]-u[i-1] )/( 2*h );
}

H_DBL fda_D1_3_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 3-point scheme, external node, order O(h^2) */
  return sgn(i)*( -3*u[sgn(i)*i]+4*u[sgn(i)*(i+1)]-u[sgn(i)*(i+2)] )/( 2*h );
}



H_DBL fda_D1_eon_3 ( H_DBL *u, H_DBL h, int N, int i )
{
  /** Inner node */
  if(i>0 && i<N-1) return fda_D1_3_inner_node(u, h, i);
  
  /** External and next to external nodes */
  else if(i==0) return fda_D1_3_extern_0_node(u, h, i);
  else return fda_D1_3_extern_0_node(u, h, -i); /* i=N-1 */
}


/**
 *
 * Second order derivs
 * 
 */
H_DBL fda_D2_3_inner_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 3-point scheme, inner node, order O(h^2) */
  return ( u[i+1]-2*u[i]+u[i-1] )/( h*h );
}

H_DBL fda_D2_3_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 3-point scheme, extarnal node, order O(h^1) */
  return ( u[sgn(i)*i]-2*u[sgn(i)*(i+1)]+u[sgn(i)*(i+2)] )/( h*h );
}

H_DBL fda_D2_4_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 4-point scheme, extarnal node, order O(h^2) */
  return ( 2*u[sgn(i)*i]-5*u[sgn(i)*(i+1)]+4*u[sgn(i)*(i+2)]-u[sgn(i)*(i+3)] )/( h*h );
}


H_DBL fda_D2_enn_3 ( H_DBL *u, H_DBL h, int N, int i )
{
  /** Inner node */
  if(i>0 && i<N-1) return fda_D2_3_inner_node(u, h, i);
  
  /** External nodes */
  else if(i==0) return fda_D2_3_extern_0_node(u, h, i);
  else return fda_D2_3_extern_0_node(u, h, -i); /* i=N-1 */
}


H_DBL fda_D2_eon_3 ( H_DBL *u, H_DBL h, int N, int i )
{
  /*  3 point second spatial derivative stecil, generally speaking it's  */
  /*  3/4 point second derivative stecil - of order O(h^2), where h is */
  /*  mesh step size */

  /** Inner node */
  if(i>0 && i<N-1) return fda_D2_3_inner_node(u, h, i);
  
  /** External nodes */
  else if(i==0) return fda_D2_4_extern_0_node(u, h, i);
  else return fda_D2_4_extern_0_node(u, h, -i); /* i=N-1 */
}
