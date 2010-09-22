/**
 * @file   flag_criterion.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Tue Sep  7 16:41:24 2010
 * 
 * @brief  This file contains all available flagging criterion
 * functions.
 * 
 * 
 */


#include "flag_criterion.h"


int h_fc_Test ( void *vm, H_DBL * tau )
{
  int i ,N;

  h_hms *m = (h_hms *)vm;

  printf("\nh_fc_Test\n\n");
  
  N = m->g->N;

  for (i = 0; i < N; i++) {
      tau[i] = sin(2*i)*10;
  }
  
  return H_OK;
}

int h_fc_Richardson ( h_grid * g, h_amrp * p, H_DBL * tau ){
  return H_ER;
}

int h_fc_Energy ( h_grid * g, h_amrp * p, H_DBL * tau ){
  return H_ER;
}

int h_fc_SV ( h_grid * g, h_amrp * p, H_DBL * tau ){
  return H_ER;
}

int h_fc_TV ( h_grid * g, h_amrp * p, H_DBL * tau ){
  return H_ER;
}
