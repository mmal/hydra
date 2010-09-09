#include "create_grid.h"


void h_gen_grid_family ( h_grid * cg, h_amrp *p )
{
  int l;
  int lmax = p->lmax;

  h_grid * g = cg;
  
  for (l = 1; l < lmax; l++) {
      h_gen_off_grid ( g, p, l );
      g = (h_grid*) g->offspring;
  }
}


void h_gen_off_grid ( h_grid *cg, h_amrp *p, int l )
{
  int rr = p->rr;
  int buf = p->buf;

  int cN = cg->N;
  int fN;
  
  int id[2] = { 4, 5};
  int Nid = 2;


  H_DBL xL, xR;
  H_DBL ch = cg->h;
  
  h_grid * fg = h_alloc_grid ( );


  if ( id[0]-buf<0 )
      xL = cg->xL;
  else
      xL = cg->x[id[0]-buf];

  
  if ( id[Nid-1]+buf > cN-1 )
      xR = cg->xR;
  else
      xR = cg->x[id[Nid-1]+buf];
  
  xL = cg->x[id[0]];
  xR = cg->x[id[Nid-1]];

  xL = -0.2;
  xR = 0.;

  printf(" l=%d, cg->xL=%f, cg->xR=%f\n", l, cg->xL, cg->xR );

  printf(" l=%d, xL=%f, xR=%f\n", l, xL, xR );
  
  fN = (int) ( lround( (xR-xL)/ch*rr) ) + 1;
  
  h_init_fine_grid ( fg, xL, xR, fN, cg->ord, l, 0 );
  
  cg->offspring = fg;
  fg->parent = cg;
}
