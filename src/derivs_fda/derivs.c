#include "derivs.h"


int sgn ( int a ) {
  int out;
  if ( a > 0 ) {
      out = 1;
  }
  else {
      if( a == 0 ) {
          out = 0;
      }
      else out = -1;
  }
  return out;
}

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

H_DBL fda_D1_5_inner_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 5-point scheme, inner node, order O(h^4) */
  return ( u[i-2]-8*u[i-1]+8*u[i+1]-u[i+2] )/( 12*h );
}

H_DBL fda_D1_5_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 5-point scheme, external node, order O(h^4) */
  return sgn(i)*( -25*u[sgn(i)*i]+48*u[sgn(i)*(i+1)]-36*u[sgn(i)*(i+2)]+16*u[sgn(i)*(i+3)]-3*u[sgn(i)*(i+4)] )/( 12*h );
}

H_DBL fda_D1_5_extern_1_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 5-point scheme, near external node, order O(h^4) */
  return sgn(i)*( -3*u[sgn(i)*(i-1)]-10*u[sgn(i)*i]+18*u[sgn(i)*(i+1)]-6*u[sgn(i)*(i+2)]+u[sgn(i)*(i+3)] )/( 12*h );
}

H_DBL fda_D1_7_inner_node ( H_DBL *u, H_DBL h, int i ) {
  /** First derivative, 7-point scheme, inner node, order O(h^6) */  
  return ( -u[i-3]+9*u[i-2]-45*u[i-1]+45*u[i+1]-9*u[i+2]+u[i+3] )/60/h;
}

H_DBL fda_D1_7_extern_0_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 7-point scheme, external node, order O(h^6) */
  return sgn(i)*( -147*f[sgn(i)*i]+360*f[sgn(i)*(1+i)]-450*f[sgn(i)*(2+i)]+400*f[sgn(i)*(3+i)]-225*f[sgn(i)*(4+i)]+72*f[sgn(i)*(5+i)]-10*f[sgn(i)*(6+i)] )/60/h;
}

H_DBL fda_D1_7_extern_1_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 7-point scheme, external node, order O(h^6) */
  return sgn(i)*( -10*f[sgn(i)*(-1+i)]-77*f[sgn(i)*i]+150*f[sgn(i)*(1+i)]-100*f[sgn(i)*(2+i)]+50*f[sgn(i)*(3+i)]-15*f[sgn(i)*(4+i)]+2*f[sgn(i)*(5+i)] )/60/h; 
}

H_DBL fda_D1_7_extern_2_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 7-point scheme, external node, order O(h^6) */
  return sgn(i)*( 2*f[sgn(i)*(-2+i)]-24*f[sgn(i)*(-1+i)]-35*f[sgn(i)*i]+80*f[sgn(i)*(1+i)]-30*f[sgn(i)*(2+i)]+8*f[sgn(i)*(3+i)]-f[sgn(i)*(4+i)] )/60/h;
}



H_DBL fda_D1_9_inner_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 9-point scheme, inner node, order O(h^8) */
  return ( 3.*f[-4+i]-32.*f[-3+i]+168.*f[-2+i]-672.*f[-1+i]+672.*f[1+i]-168.*f[2+i]+32.*f[3+i]-3.*f[4+i] )/840./h;
}

H_DBL fda_D1_9_extern_0_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 9-point scheme, external node, order O(h^8) */
  return sgn(i)*( -2283.*f[sgn(i)*(i)]+6720.*f[sgn(i)*(1+i)]-11760.*f[sgn(i)*(2+i)]+15680.*f[sgn(i)*(3+i)]-14700.*f[sgn(i)*(4+i)]+9408.*f[sgn(i)*(5+i)]-3920.*f[sgn(i)*(6+i)]+960.*f[sgn(i)*(7+i)]-105.*f[sgn(i)*(8+i)] )/840./h;
}

H_DBL fda_D1_9_extern_1_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 9-point scheme, external node, order O(h^8) */
  return sgn(i)*( -105.*f[sgn(i)*(-1+i)]-1338.*f[sgn(i)*(i)]+2940.*f[sgn(i)*(1+i)]-2940.*f[sgn(i)*(2+i)]+2450.*f[sgn(i)*(3+i)]-1470.*f[sgn(i)*(4+i)]+588.*f[sgn(i)*(5+i)]-140.*f[sgn(i)*(6+i)]+15.*f[sgn(i)*(7+i)] )/840./h;
}
H_DBL fda_D1_9_extern_2_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 9-point scheme, external node, order O(h^8) */
  return sgn(i)*( 15.*f[sgn(i)*(-2+i)]-240.*f[sgn(i)*(-1+i)]-798.*f[sgn(i)*(i)]+1680.*f[sgn(i)*(1+i)]-1050.*f[sgn(i)*(2+i)]+560.*f[sgn(i)*(3+i)]-210.*f[sgn(i)*(4+i)]+48.*f[sgn(i)*(5+i)]-5.*f[sgn(i)*(6+i)] )/840./h;
}
H_DBL fda_D1_9_extern_3_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 9-point scheme, external node, order O(h^8) */
  return sgn(i)*( -5.*f[sgn(i)*(-3+i)]+60.*f[sgn(i)*(-2+i)]-420.*f[sgn(i)*(-1+i)]-378.*f[sgn(i)*(i)]+1050.*f[sgn(i)*(1+i)]-420.*f[sgn(i)*(2+i)]+140.*f[sgn(i)*(3+i)]-30.*f[sgn(i)*(4+i)]+3.*f[sgn(i)*(5+i)] )/840./h;
}



H_DBL fda_D1_11_inner_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 11-point scheme, inner node, order O(h^10) */  
  return ( ( -2.*f[-5+i]+25.*f[-4+i]-150.*f[-3+i]+600.*f[-2+i]-2100.*f[-1+i]+2100.*f[1+i]-600.*f[2+i]+150.*f[3+i]-25.*f[4+i]+2.*f[5+i] )/2520./h );
}


/** External nodes */
H_DBL fda_D1_11_extern_0_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 11-point scheme, external node, order O(h^10) */
  return sgn(i)*( -7381.*f[sgn(i)*i]+25200.*f[sgn(i)*(1+i)]-56700.*f[sgn(i)*(2+i)]+100800.*f[sgn(i)*(3+i)]-132300.*f[sgn(i)*(4+i)]+127008.*f[sgn(i)*(5+i)]-88200.*f[sgn(i)*(6+i)]+43200.*f[sgn(i)*(7+i)]-14175.*f[sgn(i)*(8+i)]+2800.*f[sgn(i)*(9+i)]-252.*f[sgn(i)*(10+i)] )/2520./h;
}
H_DBL fda_D1_11_extern_1_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 11-point scheme, external node, order O(h^10) */
  return sgn(i)*( (-252.*f[sgn(i)*(-1+i)]-4609.*f[sgn(i)*i]+11340.*f[sgn(i)*(1+i)]-15120.*f[sgn(i)*(2+i)]+17640.*f[sgn(i)*(3+i)]-15876.*f[sgn(i)*(4+i)]+10584.*f[sgn(i)*(5+i)]-5040.*f[sgn(i)*(6+i)]+1620.*f[sgn(i)*(7+i)]-315.*f[sgn(i)*(8+i)]+28.*f[sgn(i)*(9+i)]) )/2520./h;
}
H_DBL fda_D1_11_extern_2_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 11-point scheme, external node, order O(h^10) */
  return sgn(i)*( (28.*f[sgn(i)*(-2+i)]-560.*f[sgn(i)*(-1+i)]-3069.*f[sgn(i)*i]+6720.*f[sgn(i)*(1+i)]-5880.*f[sgn(i)*(2+i)]+4704.*f[sgn(i)*(3+i)]-2940.*f[sgn(i)*(4+i)]+1344.*f[sgn(i)*(5+i)]-420.*f[sgn(i)*(6+i)]+80.*f[sgn(i)*(7+i)]-7.*f[sgn(i)*(8+i)]) )/2520./h;
}
H_DBL fda_D1_11_extern_3_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 11-point scheme, external node, order O(h^10) */
  return sgn(i)*( (-7.*f[sgn(i)*(-3+i)]+105.*f[sgn(i)*(-2+i)]-945.*f[sgn(i)*(-1+i)]-1914.*f[sgn(i)*i]+4410.*f[sgn(i)*(1+i)]-2646.*f[sgn(i)*(2+i)]+1470.*f[sgn(i)*(3+i)]-630.*f[sgn(i)*(4+i)]+189.*f[sgn(i)*(5+i)]-35.*f[sgn(i)*(6+i)]+3.*f[sgn(i)*(7+i)]) )/2520./h;
}
H_DBL fda_D1_11_extern_4_node ( H_DBL * f, H_DBL h, int i ) {
  /** First derivative, 11-point scheme, external node, order O(h^10) */
  return sgn(i)*( (3.*f[sgn(i)*(-4+i)]-40.*f[sgn(i)*(-3+i)]+270.*f[sgn(i)*(-2+i)]-1440.*f[sgn(i)*(-1+i)]-924.*f[sgn(i)*i]+3024.*f[sgn(i)*(1+i)]-1260.*f[sgn(i)*(2+i)]+480.*f[sgn(i)*(3+i)]-135.*f[sgn(i)*(4+i)]+24.*f[sgn(i)*(5+i)]-2.*f[sgn(i)*(6+i)]) )/2520./h;
}


H_DBL fda_D1_eon_3 ( H_DBL *u, H_DBL h, int N, int i )
{
  /** Inner node */
  if(i>0 && i<N-1) return fda_D1_3_inner_node(u, h, i);
  
  /** External and next to external nodes */
  else if(i==0) return fda_D1_3_extern_0_node(u, h, i);
  else return fda_D1_3_extern_0_node(u, h, -i); /* i=N-1 */
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

/* H_DBL fda_D1_eon_7 ( H_DBL * u, int N, int i ) { */
/*   int i_max = params->i_max; */
/*   H_DBL h = 1./(N-1); */
  
/*   /\** Inner nodes *\/ */
/*   if(i>=3 && i<=i_max-3) return fda_D1_7_inner_node( u, h, i ); */
  
/*   /\** External nodes *\/ */
/*   else if(i==0) return fda_D1_7_extern_0_node( u, h, i ); */
/*   else if(i==1) return fda_D1_7_extern_1_node( u, h, i ); */
/*   else if(i==2) return fda_D1_7_extern_2_node( u, h, i ); */
/*   else if(i==i_max-2) return fda_D1_7_extern_2_node( u, h, -i ); */
/*   else if(i==i_max-1) return fda_D1_7_extern_1_node( u, h, -i ); */
/*   else return fda_D1_7_extern_0_node(u, h, -i); /\** i == i_max *\/ */
/* } */


/* H_DBL fda_D1_eon_9 ( H_DBL * u, int N, int i ) { */
/*   int i_max = params->i_max; */
/* H_DBL h = 1./(N-1); */
  
/*   /\** Inner nodes *\/ */
/*   if(i>=4 && i<=i_max-4) return fda_D1_9_inner_node( u, h, i ); */
  
/*   /\** External nodes *\/ */
/*   else if(i==0) return fda_D1_9_extern_0_node( u, h, i ); */
/*   else if(i==1) return fda_D1_9_extern_1_node( u, h, i ); */
/*   else if(i==2) return fda_D1_9_extern_2_node( u, h, i ); */
/*   else if(i==3) return fda_D1_9_extern_3_node( u, h, i ); */
/*   else if(i==i_max-3) return fda_D1_9_extern_3_node( u, h, -i ); */
/*   else if(i==i_max-2) return fda_D1_9_extern_2_node( u, h, -i ); */
/*   else if(i==i_max-1) return fda_D1_9_extern_1_node( u, h, -i ); */
/*   else return fda_D1_9_extern_0_node(u, h, -i); /\** i == i_max *\/ */
/* } */

/* H_DBL fda_D1_eon_11 ( H_DBL * u, int N, int i ) { */
/*   int i_max = params->i_max; */
/*   H_DBL h = 1./(N-1); */
  
/*   /\** Inner nodes *\/ */
/*   if(i>=5 && i<=i_max-5) return fda_D1_7_inner_node( u, h, i ); */
  
/*   /\** External nodes *\/ */
/*   else if(i==0) return fda_D1_11_extern_0_node( u, h, i ); */
/*   else if(i==1) return fda_D1_11_extern_1_node( u, h, i ); */
/*   else if(i==2) return fda_D1_11_extern_2_node( u, h, i ); */
/*   else if(i==3) return fda_D1_11_extern_3_node( u, h, i ); */
/*   else if(i==4) return fda_D1_11_extern_4_node( u, h, i ); */
/*   else if(i==i_max-4) return fda_D1_11_extern_4_node( u, h, -i ); */
/*   else if(i==i_max-3) return fda_D1_11_extern_3_node( u, h, -i ); */
/*   else if(i==i_max-2) return fda_D1_11_extern_2_node( u, h, -i ); */
/*   else if(i==i_max-1) return fda_D1_11_extern_1_node( u, h, -i ); */
/*   else return fda_D1_11_extern_0_node(u, h, -i); /\** i == i_max *\/ */
/* } */

/* H_DBL fda_D1_eon ( H_DBL * u, int N, int i, int num ) { */
/*   if ( num == 3 ) return fda_D1_eon_3 ( u, params, i ); */
/*   else if ( num == 5 ) return fda_D1_eon_5 ( u, params, i); */
/*   else if ( num == 7 ) return fda_D1_eon_7 ( u, params, i); */
/*   else if ( num == 9 ) return fda_D1_eon_9 ( u, params, i); */
/*   else if ( num == 11 ) return fda_D1_eon_11 ( u, params, i); */
/*   else { */
/*       printf("Error: higher order schemes are not defined.\n"); */
/*       exit( EXIT_FAILURE ); */
/*   } */
/* } */

/* H_DBL fda_D1 ( H_DBL *u, int N, int i, int num ) { */
/*   int deror = 1; */
/*   int i_max = params->i_max; */
  
/*   /\** Checking first important condition *\/ */
/*   if ( num <= deror ) { */
/*       printf("Error: the number of nodes must be greater than\nthe order of derivative\n"); */
/*       exit( EXIT_FAILURE ); */
/*   } */
/*   /\** Checking second important condition *\/ */
/*   if ( i_max <= num ) { */
/*       printf("Error: the number of nodes must be less than\nthe number of spatial grid points\n"); */
/*       exit( EXIT_FAILURE ); */
/*   } */
/*   return fda_D1_eon ( u, params, i, num ); /\* fda D1 equal order nodes *\/ */
/* /\*   return fda_D1_eon_9 ( u, params, i); *\/ */
/* } */


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


H_DBL fda_D2_5_inner_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 5-point scheme, inner node, order O(h^4) */
  return (-u[i-2]+16*u[i-1]-30*u[i]+16*u[i+1]-u[i+2])/(12*h*h);
}

H_DBL fda_D2_5_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 5-point scheme, external node, order O(h^3) */
  return (35*u[sgn(i)*i]-104*u[sgn(i)*(i+1)]+114*u[sgn(i)*(i+2)]-56*u[sgn(i)*(i+3)]+11*u[sgn(i)*(i+4)])/(12*h*h);
}

H_DBL fda_D2_5_extern_1_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 5-point scheme, near external node, order O(h^3) */
  return (11*u[sgn(i)*(i-1)]-20*u[sgn(i)*i]+6*u[sgn(i)*(i+1)]+4*u[sgn(i)*(i+2)]-u[sgn(i)*(i+3)])/(12*h*h);
}

H_DBL fda_D2_6_extern_0_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 6-point scheme, external node, order O(h^4) */
  return (45*u[sgn(i)*i]-154*u[sgn(i)*(i+1)]+214*u[sgn(i)*(i+2)]-156*u[sgn(i)*(i+3)]+61*u[sgn(i)*(i+4)]-10*u[sgn(i)*(i+5)])/(12*h*h);
}

H_DBL fda_D2_6_extern_1_node ( H_DBL *u, H_DBL h, int i ) {
  /** Second derivative, 6-point scheme, near external node, order O(h^4) */
  return (10*u[sgn(i)*(i-1)]-15*u[sgn(i)*i]-4*u[sgn(i)*(i+1)]+14*u[sgn(i)*(i+2)]-6*u[sgn(i)*(i+3)]+u[sgn(i)*(i+4)])/(12*h*h);
}

H_DBL fda_D2_7_inner_node ( H_DBL * f, H_DBL h, int i ) {
  /** Second derivative, 7-point scheme, inner node, order O(h^6) */
  H_DBL h2 = h*h;
  return ( 2*f[-3+i]-27*f[-2+i]+270*f[-1+i]-490*f[i]+270*f[1+i]-27*f[2+i]+2*f[3+i] )/180/h2;
}

H_DBL fda_D2_8_extern_0_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 8-point scheme, external node, order O(h^6) */
  H_DBL h2 = h*h;
  return ( 938*f[sgn(i)*i]-4014*f[sgn(i)*(1+i)]+7911*f[sgn(i)*(2+i)]-9490*f[sgn(i)*(3+i)]+7380*f[sgn(i)*(4+i)]-3618*f[sgn(i)*(5+i)]+1019*f[sgn(i)*(6+i)]-126*f[sgn(i)*(7+i)] )/180/h2;
}

H_DBL fda_D2_8_extern_1_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 8-point scheme, external node, order O(h^6) */
  H_DBL h2 = h*h;
  return ( 126*f[sgn(i)*(-1+i)]-70*f[sgn(i)*i]-486*f[sgn(i)*(1+i)]+855*f[sgn(i)*(2+i)]-670*f[sgn(i)*(3+i)]+324*f[sgn(i)*(4+i)]-90*f[sgn(i)*(5+i)]+11*f[sgn(i)*(6+i)] )/180/h2;
}

H_DBL fda_D2_8_extern_2_node ( H_DBL * f, H_DBL h, int i ) {
  /** Second derivative, 8-point scheme, external node, order O(h^6) */
  H_DBL h2 = h*h;
  return (-11*f[sgn(i)*(-2+i)]+214*f[sgn(i)*(-1+i)]-378*f[sgn(i)*i]+130*f[sgn(i)*(1+i)]+85*f[sgn(i)*(2+i)]-54*f[sgn(i)*(3+i)]+16*f[sgn(i)*(4+i)]-2*f[sgn(i)*(5+i)] )/180/h2;
}


H_DBL fda_D2_9_inner_node ( H_DBL * f, H_DBL h, int i ) {
  /** Second derivative, 9-point scheme, inner node, order O(h^8) */
  H_DBL h2 = h*h;
  return (-9.*f[-4+i]+128.*f[-3+i]-1008.*f[-2+i]+8064.*f[-1+i]-14350.*f[i]+8064.*f[1+i]-1008.*f[2+i]+128.*f[3+i]-9.*f[4+i] )/5040./h2;
}

H_DBL fda_D2_9_extern_0_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 10-point scheme, external node, order O(h^8) */
  H_DBL h2 = h*h;
  return ( 32575.*f[sgn(i)*(i)]-165924.*f[sgn(i)*(1+i)]+422568.*f[sgn(i)*(2+i)]-704368.*f[sgn(i)*(3+i)]+818874.*f[sgn(i)*(4+i)]-667800.*f[sgn(i)*(5+i)]+375704.*f[sgn(i)*(6+i)]-139248.*f[sgn(i)*(7+i)]+30663.*f[sgn(i)*(8+i)]-3044.*f[sgn(i)*(9+i)] )/5040./h2;
}

H_DBL fda_D2_9_extern_1_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 10-point scheme, external node, order O(h^8) */
  H_DBL h2 = h*h;
  return ( 3044.*f[sgn(i)*(-1+i)]+2135.*f[sgn(i)*(i)]-28944.*f[sgn(i)*(1+i)]+57288.*f[sgn(i)*(2+i)]-65128.*f[sgn(i)*(3+i)]+51786.*f[sgn(i)*(4+i)]-28560.*f[sgn(i)*(5+i)]+10424.*f[sgn(i)*(6+i)]-2268.*f[sgn(i)*(7+i)]+223.*f[sgn(i)*(8+i)] )/5040./h2;
}

H_DBL fda_D2_9_extern_2_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 10-point scheme, external node, order O(h^8) */
  H_DBL h2 = h*h;
  return ( -223.*f[sgn(i)*(-2+i)]+5274.*f[sgn(i)*(-1+i)]-7900.*f[sgn(i)*(i)]-2184.*f[sgn(i)*(1+i)]+10458.*f[sgn(i)*(2+i)]-8932.*f[sgn(i)*(3+i)]+4956.*f[sgn(i)*(4+i)]-1800.*f[sgn(i)*(5+i)]+389.*f[sgn(i)*(6+i)]-38.*f[sgn(i)*(7+i)] )/5040./h2;
}

H_DBL fda_D2_9_extern_3_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 10-point scheme, external node, order O(h^8) */
  H_DBL h2 = h*h;
  return ( 38.*f[sgn(i)*(-3+i)]-603.*f[sgn(i)*(-2+i)]+6984.*f[sgn(i)*(-1+i)]-12460.*f[sgn(i)*(i)]+5796.*f[sgn(i)*(1+i)]+882.*f[sgn(i)*(2+i)]-952.*f[sgn(i)*(3+i)]+396.*f[sgn(i)*(4+i)]-90.*f[sgn(i)*(5+i)]+9.*f[sgn(i)*(6+i)] )/5040./h2;
}




H_DBL fda_D2_11_inner_node ( H_DBL * f, H_DBL h, int i ) {
  /** Second derivative, 11-point scheme, inner node, order O(h^10) */
  H_DBL h2 = h*h;
  return (8.*f[-5+i]-125.*f[-4+i]+1000.*f[-3+i]-6000.*f[-2+i]+42000.*f[-1+i]-73766.*f[i]+42000.*f[1+i]-6000.*f[2+i]+1000.*f[3+i]-125.*f[4+i]+8.*f[5+i])/25200./h2;
}

H_DBL fda_D2_11_extern_0_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 12-point scheme, external node, order O(h^10) */
  H_DBL h2 = h*h;
  return ( 190553.*f[sgn(i)*(i)]-1119820.*f[sgn(i)*(1+i)]+3492550.*f[sgn(i)*(2+i)]-7447100.*f[sgn(i)*(3+i)]+11517150.*f[sgn(i)*(4+i)]-13132056.*f[sgn(i)*(5+i)]+11072740.*f[sgn(i)*(6+i)]-6835800.*f[sgn(i)*(7+i)]+3009225.*f[sgn(i)*(8+i)]-895900.*f[sgn(i)*(9+i)]+161878.*f[sgn(i)*(10+i)]-13420.*f[sgn(i)*(11+i)] )/25200./h2;
}

H_DBL fda_D2_11_extern_1_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 12-point scheme, external node, order O(h^10) */
  H_DBL h2 = h*h;
  return ( 13420.*f[sgn(i)*(-1+i)]+29513.*f[sgn(i)*(i)]-234100.*f[sgn(i)*(1+i)]+540150.*f[sgn(i)*(2+i)]-804200.*f[sgn(i)*(3+i)]+888510.*f[sgn(i)*(4+i)]-731976.*f[sgn(i)*(5+i)]+444100.*f[sgn(i)*(6+i)]-192900.*f[sgn(i)*(7+i)]+56825.*f[sgn(i)*(8+i)]-10180.*f[sgn(i)*(9+i)]+838.*f[sgn(i)*(10+i)] )/25200./h2;
}

H_DBL fda_D2_11_extern_2_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 12-point scheme, external node, order O(h^10) */
  H_DBL h2 = h*h;
  return ( -838.*f[sgn(i)*(-2+i)]+23476.*f[sgn(i)*(-1+i)]-25795.*f[sgn(i)*(i)]-49740.*f[sgn(i)*(1+i)]+125340.*f[sgn(i)*(2+i)]-140504.*f[sgn(i)*(3+i)]+114198.*f[sgn(i)*(4+i)]-68280.*f[sgn(i)*(5+i)]+29290.*f[sgn(i)*(6+i)]-8540.*f[sgn(i)*(7+i)]+1517.*f[sgn(i)*(8+i)]-124.*f[sgn(i)*(9+i)] )/25200./h2;
}

H_DBL fda_D2_11_extern_3_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 12-point scheme, external node, order O(h^10) */
  H_DBL h2 = h*h;
  return ( 124.*f[sgn(i)*(-3+i)]-2326.*f[sgn(i)*(-2+i)]+31660.*f[sgn(i)*(-1+i)]-53075.*f[sgn(i)*(i)]+11640.*f[sgn(i)*(1+i)]+27132.*f[sgn(i)*(2+i)]-25928.*f[sgn(i)*(3+i)]+15990.*f[sgn(i)*(4+i)]-6900.*f[sgn(i)*(5+i)]+2010.*f[sgn(i)*(6+i)]-356.*f[sgn(i)*(7+i)]+29.*f[sgn(i)*(8+i)] )/25200./h2;
}

H_DBL fda_D2_11_extern_4_node ( H_DBL * f, H_DBL h, int i ) {
   /** Second derivative, 12-point scheme, external node, order O(h^10) */
  H_DBL h2 = h*h;
  return ( -29.*f[sgn(i)*(-4+i)]+472.*f[sgn(i)*(-3+i)]-4240.*f[sgn(i)*(-2+i)]+38040.*f[sgn(i)*(-1+i)]-67430.*f[sgn(i)*(i)]+34608.*f[sgn(i)*(1+i)]+336.*f[sgn(i)*(2+i)]-2960.*f[sgn(i)*(3+i)]+1635.*f[sgn(i)*(4+i)]-520.*f[sgn(i)*(5+i)]+96.*f[sgn(i)*(6+i)]-8.*f[sgn(i)*(7+i)] )/25200./h2;
}


/** */
H_DBL fda_D2_enn_3 ( H_DBL *u, H_DBL h, int N, int i )
{
  /** Inner node */
  if(i>0 && i<N-1) return fda_D2_3_inner_node(u, h, i);
  
  /** External nodes */
  else if(i==0) return fda_D2_3_extern_0_node(u, h, i);
  else return fda_D2_3_extern_0_node(u, h, -i); /* i=N-1 */
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

/* H_DBL fda_D2_enn_7 ( H_DBL *u, int N, int i ) { */
/*   return 0.; */
/* } */


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

/* H_DBL fda_D2_eon_7 ( H_DBL *u, int N, int i ) { */
/*   /\**  7 point second spatial derivative stecil, generally speaking it's  *\/ */
/*   /\*  7/8 point second derivative stecil - of order O(h^6)  *\/ */
/*   int i_max = params->i_max; */
/*   H_DBL h = 1./(N-1); */
  
/*   /\** Inner node *\/ */
/*   if( i>=3 && i<=i_max-3 ) return fda_D2_7_inner_node( u, h, i ); */
  
/*   /\** External  nodes *\/ */
/*   else if(i==0) return fda_D2_8_extern_0_node( u, h, i ); */
/*   else if(i==1) return fda_D2_8_extern_1_node( u, h, i ); */
/*   else if(i==2) return fda_D2_8_extern_2_node( u, h, i ); */
/*   else if(i==i_max-2) return fda_D2_8_extern_2_node( u, h, -i ); */
/*   else if(i==i_max-1) return fda_D2_8_extern_1_node( u, h, -i ); */
/*   else return fda_D2_8_extern_0_node( u, h, -i ); /\** i == i_max *\/ */
/* } */

/* H_DBL fda_D2_eon_9 ( H_DBL *u, int N, int i ) { */
/*   /\**  9 point second spatial derivative stecil, generally speaking it's  *\/ */
/*   /\*  9/10 point second derivative stecil - of order O(h^8)  *\/ */
/*   int i_max = params->i_max; */
/*   H_DBL h = 1./(N-1); */
  
/*   /\** Inner node *\/ */
/*   if( i>=4 && i<=i_max-4 ) return fda_D2_9_inner_node( u, h, i ); */
  
/*   /\** External  nodes *\/ */
/*   else if(i==0) return fda_D2_9_extern_0_node( u, h, i ); */
/*   else if(i==1) return fda_D2_9_extern_1_node( u, h, i ); */
/*   else if(i==2) return fda_D2_9_extern_2_node( u, h, i ); */
/*   else if(i==3) return fda_D2_9_extern_3_node( u, h, i ); */
/*   else if(i==i_max-3) return fda_D2_9_extern_3_node( u, h, -i ); */
/*   else if(i==i_max-2) return fda_D2_9_extern_2_node( u, h, -i ); */
/*   else if(i==i_max-1) return fda_D2_9_extern_1_node( u, h, -i ); */
/*   else return fda_D2_9_extern_0_node( u, h, -i ); /\** i == i_max *\/ */
/* } */


/* H_DBL fda_D2_eon_11 ( H_DBL *u, int N, int i ) { */
/*   /\**  11 point second spatial derivative stecil, generally speaking it's  *\/ */
/*   /\*  11/12 point second derivative stecil - of order O(h^10)  *\/ */
/*   int i_max = params->i_max; */
/*   H_DBL h = 1./(N-1); */
  
/*   /\** Inner node *\/ */
/*   if( i>=5 && i<=i_max-5 ) return fda_D2_11_inner_node( u, h, i ); */
  
/*   /\** External  nodes *\/ */
/*   else if(i==0) return fda_D2_11_extern_0_node( u, h, i ); */
/*   else if(i==1) return fda_D2_11_extern_1_node( u, h, i ); */
/*   else if(i==2) return fda_D2_11_extern_2_node( u, h, i ); */
/*   else if(i==3) return fda_D2_11_extern_3_node( u, h, i ); */
/*   else if(i==4) return fda_D2_11_extern_4_node( u, h, i ); */
/*   else if(i==i_max-4) return fda_D2_11_extern_4_node( u, h, -i ); */
/*   else if(i==i_max-3) return fda_D2_11_extern_3_node( u, h, -i ); */
/*   else if(i==i_max-2) return fda_D2_11_extern_2_node( u, h, -i ); */
/*   else if(i==i_max-1) return fda_D2_11_extern_1_node( u, h, -i ); */
/*   else return fda_D2_11_extern_0_node( u, h, -i ); /\** i == i_max *\/ */
/* } */


/* H_DBL fda_D2_eon ( H_DBL *u, int N, int i, int num ) { */
/*   if ( num == 3 ) return fda_D2_eon_3 ( u, params, i ); */
/*   else if ( num == 5 ) return fda_D2_eon_5 ( u, params, i); */
/*   else if ( num == 7 ) return fda_D2_eon_7 ( u, params, i); */
/*   else if ( num == 9 ) return fda_D2_eon_9 ( u, params, i); */
/*   else if ( num == 11 ) return fda_D2_eon_11 ( u, params, i); */
/*   else { */
/*       printf("Error: higher order schemes are not defined.\n"); */
/*       exit( EXIT_FAILURE ); */
/*   } */
/* } */

/* H_DBL fda_D2 ( H_DBL *u, int N, int i, int num ) { */
/*   int deror = 2; */
/*   int i_max = params->i_max; */
  
/*   /\** Checking first important condition *\/ */
/*   if ( num <= deror ) { */
/*       printf("Error: the number of nodes must be greater than\nthe order of derivative\n"); */
/*       exit( EXIT_FAILURE ); */
/*   } */
/*   /\** Checking second important condition *\/ */
/*   if ( i_max <= num ) { */
/*       printf("Error: the number of nodes must be less than\nthe number of spatial grid points\n"); */
/*       exit( EXIT_FAILURE ); */
/*   } */
/*   return fda_D2_eon ( u, params, i, num ); */
/* /\*   return fda_D2_eon_9 ( u, params, i); *\/ */
/* } */



/* /\** */
/*  * */
/*  * Higher order derivs */
/*  *  */
/*  *\/ */
/* H_DBL fda_D3_9_inner_node ( H_DBL *f, H_DBL h, int i ) { */
/*   /\** Third derivative, 9-point scheme, inner node, order O(h^6) *\/ */
/*   return (-7*f[-4+i]+72*f[-3+i]-338*f[-2+i]+488*f[-1+i]-488*f[1+i]+338*f[2+i]-72*f[3+i]+7*f[4+i])/(240*pow(h, 3.)); */
/* } */

/* H_DBL fda_D3_7_extern_0_node ( H_DBL * f, H_DBL h, int i ) { */
/*    /\** Third derivative, 7-point scheme, external node, order O(h^4) *\/ */
/*   H_DBL h3 = h*h*h; */
/*   return sgn(i)*(15.*f[sgn(i)*(-6+i)]-104.*f[sgn(i)*(-5+i)]+307.*f[sgn(i)*(-4+i)]-496.*f[sgn(i)*(-3+i)]+461.*f[sgn(i)*(-2+i)]-232.*f[sgn(i)*(-1+i)]+49.*f[sgn(i)*(i)])/(8.*h3); */
/* } */

/* H_DBL fda_D4_9_inner_node ( H_DBL *f, H_DBL h, int i ) { */
/*   /\** Fourth derivative, 9-point scheme, inner node, order O(h^6) *\/ */
/*   return (7*f[-4+i]-96*f[-3+i]+676*f[-2+i]-1952*f[-1+i]+2730*f[i]-1952*f[1+i]+676*f[2+i]-96*f[3+i]+7*f[4+i])/(240*pow(h, 4.)); */
/* } */

/* H_DBL fda_D5_11_inner_node ( H_DBL *f, H_DBL h, int i ) { */
/*   /\** Fifth derivative, 11-point scheme, inner node, order O(h^6) *\/ */
/*   return (-13*f[-5+i]+152*f[-4+i]-783*f[-3+i]+1872*f[-2+i]-1938*f[-1+i]+1938*f[1+i]-1872*f[2+i]+783*f[3+i]-152*f[4+i]+13*f[5+i])/(288*pow(h, 5.)); */
/* } */

/* H_DBL fda_D6_11_inner_node ( H_DBL *f, H_DBL h, int i ) { */
/*   /\** Sixth derivative, 11-point scheme, inner node, order O(h^6) *\/ */
/*   return (13*f[-5+i]-190*f[-4+i]+1305*f[-3+i]-4680*f[-2+i]+9690*f[-1+i]-12276*f[i]+9690*f[1+i]-4680*f[2+i]+1305*f[3+i]-190*f[4+i]+13*f[5+i])/(240*pow(h, 6.)); */
/* } */
