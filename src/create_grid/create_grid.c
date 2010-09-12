

#include "create_grid.h"


void _h_create_child_grid ( h_grid *parent, h_grid **child,
                            h_amrp *p, int m, int idL, int idR )
{
  /* ghost points, what with them ? */ 
  int rr = p->rr;
  int Nf = (idR-idL)*rr+1;

  int Lghost_p = parent->Lghost; /* number of left ghost
                                  * points of the parent grid */ 
  int Rghost_p = parent->Rghost; /* number of right ghost
                                  * points of the parent grid */ 
  
  int Lghost, Rghost; /* left and right ghost points of the child grid */
  
  H_DBL xL = parent->x[idL];
  H_DBL xR = parent->x[idR];

  
  h_grid *fineg = h_alloc_grid ( );

  h_init_grid ( fineg, xL, xR, Nf, 0, 0, parent->rank, (parent->l)+1, m );
  
  /* if( m != 0 ) */
  /*     fineg->Lsibling = parent->children[m]; */
  /* if( m != parent->Nchildren-1 ) */
  /*     fineg->Rsibling = parent->children[m+1]; */
  printf("create_grid.c m=%d, fineg->xL=%f, fineg->xR=%f\n",
         m, fineg->xL, fineg->xR );
  
  fineg->parent = (h_grid*) parent;

  *child = fineg;
}
                            
void _h_create_offspring_grids ( h_grid *cg, h_amrp *p,
                                int *idL, int *idR, int Ngrids )
{
  /* h_grid *g = fg; */
  int m;
  
  h_grid **child = (h_grid **) malloc( Ngrids*sizeof( h_grid *) );

  /* CAUSED FREE ERROR */
  /* for (m = 0; m < Ngrids; m++) { */
  /*     child[m] = h_alloc_grid ( ); */
  /* } */

  for (m = 0; m < Ngrids; m++) {
      _h_create_child_grid ( cg, &child[m], p, m, idL[m], idR[m] );
  }
  
  cg->children = (void **) child;
  cg->Nchildren = Ngrids;
}


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
  
  h_init_grid ( fg, xL, xR, fN, 0, 0, cg->rank, l, 0 );
  
  cg->offspring = fg;
  fg->parent = cg;
}
