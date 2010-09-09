#ifndef __DERIVS_H__
#define __DERIVS_H__

#include "src/common/types.h"

int sgn ( int a );
/**
 *
 * Dissipative operators
 *
 */

/**
 *
 * First order derivs
 *
 */
H_DBL fda_D1_3_inner_node ( H_DBL * u, H_DBL h, int i );
H_DBL fda_D1_3_extern_0_node ( H_DBL * u, H_DBL h, int i );
H_DBL fda_D1_5_inner_node ( H_DBL * u, H_DBL h, int i );
H_DBL fda_D1_5_extern_0_node ( H_DBL * u, H_DBL h, int i );
H_DBL fda_D1_5_extern_1_node ( H_DBL * u, H_DBL h, int i );
H_DBL fda_D1_7_inner_node ( H_DBL * u, H_DBL h, int i );
H_DBL fda_D1_7_extern_0_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_7_extern_1_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_7_extern_2_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_9_inner_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_9_extern_0_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_9_extern_1_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_9_extern_2_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_9_extern_3_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_9_inner_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_11_inner_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_11_extern_0_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_11_extern_1_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_11_extern_2_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_11_extern_3_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_11_extern_4_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D1_11_inner_node ( H_DBL * f, H_DBL h, int i );

/** */
H_DBL fda_D1_eon_3 ( H_DBL * u, H_DBL h, int N, int i );
H_DBL fda_D1_eon_5 ( H_DBL * u, H_DBL h, int N, int i );
H_DBL fda_D1_eon_7 ( H_DBL * u, H_DBL h, int N, int i );
H_DBL fda_D1_eon_9 ( H_DBL * u, H_DBL h, int N, int i );
H_DBL fda_D1_eon_11 ( H_DBL * u, H_DBL h, int N, int i );
H_DBL fda_D1_eon ( H_DBL * u, H_DBL h, int N, int i, int num );
H_DBL fda_D1 ( H_DBL * u, H_DBL h, int N, int i, int num );
/**
 *
 * Second order derivs
 * 
 */
H_DBL fda_D2_3_inner_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_3_extern_0_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_4_extern_0_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_5_inner_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_5_extern_0_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_5_extern_1_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_6_extern_0_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_6_extern_1_node ( H_DBL *u, H_DBL h, int i );
H_DBL fda_D2_7_inner_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_8_extern_0_node ( H_DBL *f, H_DBL h, int i );
H_DBL fda_D2_8_extern_1_node ( H_DBL *f, H_DBL h, int i );
H_DBL fda_D2_8_extern_2_node ( H_DBL *f, H_DBL h, int i );
H_DBL fda_D2_9_inner_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_9_extern_0_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_9_extern_1_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_9_extern_2_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_9_extern_3_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_11_inner_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_11_extern_0_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_11_extern_1_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_11_extern_2_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_11_extern_3_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D2_11_extern_4_node ( H_DBL * f, H_DBL h, int i );

/** */
H_DBL fda_D2_enn_3 ( H_DBL *u, H_DBL h, int N, int i );
H_DBL fda_D2_enn_5 ( H_DBL *u, H_DBL h, int N, int i );
/* H_DBL fda_D2_enn_7 ( H_DBL *u, int N, int i ); */

H_DBL fda_D2_eon_3 ( H_DBL *u, H_DBL h, int N, int i );
H_DBL fda_D2_eon_5 ( H_DBL *u, H_DBL h, int N, int i );
/* H_DBL fda_D2_eon_7 ( H_DBL *u, int N, int i ); */

/* H_DBL fda_D2_eon_9 ( H_DBL *u, int N, int i ); */
/* H_DBL fda_D2_eon_11 ( H_DBL *u, int N, int i ); */
/* H_DBL fda_D2_eon ( H_DBL *u, int N, int i, int num ); */
/* H_DBL fda_D2 ( H_DBL *u, int N, int i, int num ); */
/**
 *
 * Higher order derivs
 * 
 */
H_DBL fda_D3_9_inner_node ( H_DBL *f, H_DBL h, int i );
H_DBL fda_D3_7_extern_0_node ( H_DBL * f, H_DBL h, int i );
H_DBL fda_D4_9_inner_node ( H_DBL *f, H_DBL h, int i );
H_DBL fda_D5_11_inner_node ( H_DBL *f, H_DBL h, int i );
H_DBL fda_D6_11_inner_node ( H_DBL *f, H_DBL h, int i );


#endif /* _DERIVS_H_ */
