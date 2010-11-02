

#include "derivs.h"



/**
 *
 * First order derivs
 *
 */
H_DBL fda_D1_5_inner_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 5-point scheme, inner node, order O(h^4) */
  return ( u[i-2]-8*u[i-1]+8*u[i+1]-u[i+2] )/( 12*h );
}

H_DBL fda_D1_5_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 5-point scheme, external node, order O(h^4) */
  return sgn(i)*( -25*u[sgn(i)*i]+48*u[sgn(i)*(i+1)]
                  -36*u[sgn(i)*(i+2)]+16*u[sgn(i)*(i+3)]
                  -3*u[sgn(i)*(i+4)] )/( 12*h );
}

H_DBL fda_D1_5_extern_1_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 5-point scheme, near external node, order O(h^4) */
  return sgn(i)*( -3*u[sgn(i)*(i-1)]-10*u[sgn(i)*i]
                  +18*u[sgn(i)*(i+1)]-6*u[sgn(i)*(i+2)]
                  +u[sgn(i)*(i+3)] )/( 12*h );
}

H_DBL fda_D1_eon_5 ( H_DBL * u, H_DBL h, int N, int i )
{
  /** Inner nodes */
  if( i>1 && i<N-2 ) return fda_D1_5_inner_node(u, h, i);
  
  /** External nodes */
  else if(i==0) return fda_D1_5_extern_0_node(u, h, i);
  else if(i==1) return fda_D1_5_extern_1_node(u, h, i);
  else if(i==N-1) return fda_D1_5_extern_0_node(u, h, -i);
  else return fda_D1_5_extern_1_node(u, h, -i); /* i=N-2 */
}



/**
 *
 * Second order derivs
 * 
 */

H_DBL fda_D2_5_inner_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 5-point scheme, inner node, order O(h^4) */
  return (-u[i-2]+16*u[i-1]-30*u[i]+16*u[i+1]-u[i+2])/(12*h*h);
}

H_DBL fda_D2_5_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 5-point scheme, external node, order O(h^3) */
  return (35*u[sgn(i)*i]-104*u[sgn(i)*(i+1)]+114*u[sgn(i)*(i+2)]
          -56*u[sgn(i)*(i+3)]+11*u[sgn(i)*(i+4)])/(12*h*h);
}

H_DBL fda_D2_5_extern_1_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 5-point scheme, near external node, order O(h^3) */
  return (11*u[sgn(i)*(i-1)]-20*u[sgn(i)*i]+6*u[sgn(i)*(i+1)]
          +4*u[sgn(i)*(i+2)]-u[sgn(i)*(i+3)])/(12*h*h);
}

H_DBL fda_D2_6_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 6-point scheme, external node, order O(h^4) */
  return (45*u[sgn(i)*i]-154*u[sgn(i)*(i+1)]+214*u[sgn(i)*(i+2)]
          -156*u[sgn(i)*(i+3)]+61*u[sgn(i)*(i+4)]-10*u[sgn(i)*(i+5)])/(12*h*h);
}

H_DBL fda_D2_6_extern_1_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 6-point scheme, near external node, order O(h^4) */
  return (10*u[sgn(i)*(i-1)]-15*u[sgn(i)*i]-4*u[sgn(i)*(i+1)]
          +14*u[sgn(i)*(i+2)]-6*u[sgn(i)*(i+3)]+u[sgn(i)*(i+4)])/(12*h*h);
}

H_DBL fda_D2_enn_5 ( H_DBL *u, H_DBL h, int N, int i )
{
  /** Inner node */
  if(i>1 && i<N-2) return fda_D2_5_inner_node(u, h, i);
  
  /** External nodes */
  else if(i==0) return fda_D2_5_extern_0_node(u, h, i);
  else if(i==1) return fda_D2_5_extern_1_node(u, h, i);
  else if(i==N-1) return fda_D2_5_extern_0_node(u, h, -i);
  else return fda_D2_5_extern_1_node(u, h, -i); /* i=N-2 */
}

H_DBL fda_D2_eon_5 ( H_DBL *u, H_DBL h, int N, int i )
{
  /*  5 point second spatial derivative stecil, generally speaking it's  */
  /*  5/6 point second derivative stecil - of order O(h^4), where h is */
  /*  mesh step size */
  
  /** Inner node */
  if(i>1 && i<N-2) return fda_D2_5_inner_node(u, h, i);
  
  /** External nodes */
  else if(i==0) return fda_D2_6_extern_0_node(u, h, i);
  else if(i==1) return fda_D2_6_extern_1_node(u, h, i);
  else if(i==N-1) return fda_D2_6_extern_0_node(u, h, -i);
  else return fda_D2_6_extern_1_node(u, h, -i); /* i=N-2 */
}

