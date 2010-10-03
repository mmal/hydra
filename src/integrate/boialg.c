

#include "boialg.h"



int h_boialg ( h_hms *hms )
{
  char *fnc_msg = "Berger&Oliger integration algorithm";

  int l, status;

  h_gset *gset = hms->gset;
  h_amrp *amrp = hms->amrp;
  h_fnc *fnc = hms->fnc;

  if ( gset == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_gset is unallocated",
                  WARNING, 0 );
      return H_ER;
  }
  else if ( amrp == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_amrp is unallocated",
                  WARNING, 0 );
      return H_ER;
  }
  else if ( fnc == NULL ) {
      _STAT_MSG ( fnc_msg,
                  "h_fnc is unallocated",
                  WARNING, 0 );
      return H_ER;
  }
  else {

      l = 0;

      status = _h_boialg ( gset, amrp, fnc, l );

      if ( status != H_OK )
          _STAT_MSG ( fnc_msg,
                      "_h_bioalg error at level ",
                      WARNING, 0 );

      return status;
  }
}



int _h_boialg ( h_gset *gset, h_amrp *amrp, h_fnc *fnc, int l )
{
  int status, repeat, L;

  h_glevel *glevel;
  
  L = gset->L;

  repeat = (int) pow( amrp->rr, l );

  glevel = h_point_to_glevel ( gset, l );
      
  do {
      printf("repeat=%d, l=%d\n", repeat, l);
      repeat--;
          
      /* regriding, and step on level l */
      status = _h_boialg_step_glevel ( glevel, amrp, fnc );

      if ( status != H_OK )
          break;
      
      if ( l < L - 1 ) {
          
          status = _h_boialg ( gset, amrp, fnc , l+1 );
          if ( status != H_OK )
              break;

          status = _h_update_glevel ( glevel, h_point_to_glevel ( gset, l+1 ) );
          if ( status != H_OK )
              break;

      }
      
  } while ( repeat > 0 );
  
  return status;
}
