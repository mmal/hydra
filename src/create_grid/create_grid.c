

#include "create_grid.h"


void _h_create_child_grid ( h_grid *parent, h_grid **child,
                            h_amrp *p, int m, int idL, int idR )
{
  /* ghost points, what with them ? */ 
  int i;

  int rr = p->rr; /* refinement ratio */

  int sp = p->sp;

  int N_c = (idR-idL)*rr+1; /* number of child grid points */ 
  
  /* int Lghost_p = parent->Lghost; /\* number of left ghost */
  /*                                 * points of the parent grid *\/  */
  /* int Rghost_p = parent->Rghost; /\* number of right ghost */
  /*                                 * points of the parent grid *\/  */
  
  int Lghost_c = 0, Rghost_c = 0; /* left and right ghost points of the child grid */

  H_DBL h_p = parent->h; /* spatial spacing of parent grid */
  
  H_DBL xL_c = parent->x[idL]; /* coordinate of the left end of the child grid */

  H_DBL xR_c = parent->x[idR]; /* coordinate of the right end of the child grid */

  H_DBL xL_m, xR_m; /* coordinates of the ends of the master grid */
  
  h_grid *fine_grid = h_alloc_grid ( );

  h_grid *master_grid;

  VL(("create_grid.c _h_create_child_grid TEST\n"));
  
  if ( parent->is_master == H_TRUE  ) { /* parent grid is a master grid */
      VL(("create_grid.c _h_create_child_grid TEST parent->is_master == H_TRUE\n"));
      xL_m = parent->xL;
      xR_m = parent->xR;
      VL(("create_grid.c _h_create_child_grid TEST parent->is_master == H_TRUE 2\n"));
  }
  else { /* parent grid is not a master grid */
      VL(("create_grid.c _h_create_child_grid TEST parent->is_master != H_TRUE\n"));
      master_grid = (h_grid *) parent->master;
      VL(("create_grid.c _h_create_child_grid TEST parent->is_master != H_TRUE 2\n"));

      /* xL_m = master_grid->xL; */
      /* xR_m = master_grid->xR; */
      xL_m = -1.;  /* TODO: */
      xR_m = 1.;
      VL(("create_grid.c _h_create_child_grid TEST parent->is_master != H_TRUE 3\n"));
  }

  VL(("create_grid.c _h_create_child_grid TEST 2\n"));

  
  for (i = 1; i <= sp; i++) {
      if (xL_c - i*h_p/rr > xL_m)
          Lghost_c++;
      if (xR_c + i*h_p/rr < xR_m)
          Rghost_c++;
  }

  VL(("floor_sqrt(%d) == %d\n", 0, 1));  /* logs a printf style message */
  
  VL(( "create_grid.c xL_m=%f, xR_m=%f\n", xL_m, xR_m ));
  
  VL(( "create_grid.c m=%d: Lghost=%d, Rghost=%d\n", m, Lghost_c, Rghost_c ));
  
  h_init_grid ( fine_grid, xL_c, xR_c, N_c, Lghost_c, Rghost_c, parent->rank, (parent->l)+1, m );
  
  /* if( m != 0 ) */
  /*     fineg->Lsibling = parent->children[m]; */
  /* if( m != parent->Nchildren-1 ) */
  /*     fineg->Rsibling = parent->children[m+1]; */
  VL(("create_grid.c m=%d, fine_grid->xL=%f, fine_grid->xR=%f\n",
      m, fine_grid->xL, fine_grid->xR ));
  
  fine_grid->parent = (h_grid*) parent;

  *child = fine_grid;
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
  VL(("create_grid.c _h_create_offspring_grids TEST\n"));
  for (m = 0; m < Ngrids; m++) {
      VL(("create_grid.c _h_create_offspring_grids m=%d TEST\n", m));
      _h_create_child_grid ( cg, &child[m], p, m, idL[m], idR[m] );
  }
  
  
  cg->children = (void **) child;
  cg->Nchildren = Ngrids;


  /* if ( parent->is_master == H_TRUE  ) { /\* parent grid is a master grid *\/ */
  /*     child[m] = parent->xL; */
  /*     xR_m = parent->xR; */
  /* } */
  /* elseent->master != NULL ) { /\* parent grid is not a master grid *\/ */
  /*     xL_m = parent->master->xL; */
  /*     xR_m = parent->master->xR; */
  /* } */
}


void _h_create_set_of_grids ( h_grid *g, h_amrp *p, h_fnc *f )
{
  int i, l=g->l, lmax = p->lmax;
  
  int *id_fp;

  int Nfp;

  int *idL, *idR, Ngrids;

  h_grid *g_c;

  VL(("l=%d, lmax=%d\n", l, lmax));
  
  _h_acd_to_one_grid ( g, f );

  if ( l < lmax ) {
      
      h_flagging_points ( g, p, h_fc_Test, &id_fp, &Nfp );

      if ( Nfp > 0 ) {
      
          h_clustering_flagged ( id_fp, Nfp, p->buf, g->N, &idL, &idR, &Ngrids );

          if( id_fp!=NULL )
              free( id_fp );
          VL(("create_grid TEST\n")); /****/
          _h_create_offspring_grids ( g, p, idL, idR, Ngrids );

            if( idL!=NULL )
                free( idL );
            if( idR!=NULL )
                free( idR );

            for (i = 0; i < Ngrids; i++) {
                g_c = g->children[i];
                _h_create_set_of_grids ( g_c, p, f );
            }
      }
      else {
          _STAT_MSG ( "Create set of grids",
                      "no flagged points at leve l=",
                      WARNING, 0 );
      }
  }
}


void _h_acd_to_one_grid ( h_grid *g, h_fnc *f )
{
  int i, j, N;

  int Lghost, Rghost, Ntotal;

  _fnc_1D fnc_ptr;

 
  if ( g == NULL || g->x == NULL || g->u == NULL )
      _STAT_MSG ( "Assign Cauchy data to one grid",
                  "grid structure is not allocated",
                  ERROR, 0 );
  
  if ( g->rank != f->rank )
      _STAT_MSG ( "Assign Cauchy data: using given initial conditions",
                  "ranks of grid and fnc structures are not equal",
                  ERROR, 0 );
  
  N = g->N;
  Lghost = g->Lghost;
  Rghost = g->Rghost;
  Ntotal = N+Lghost+Rghost;
          

  for (i = 0; i < f->rank; i++) { /* loop through initial functions */

      fnc_ptr = f->C_da[i];
      
      for (j = 0; j < Ntotal; j++) { /* loop through the grid points */
          g->u[i*Ntotal+j] = fnc_ptr( g->x[j], f->params );
      }

  }
}



void _h_assign_cauchy_data ( h_grid *g, h_amrp *p, h_fnc *f )
{
  /* int i, j; */
  /* int l=0, N; */

  /* int lmax = p->lmax; */

  /* int Lghost, Rghost, Ntotal; */

  /* _fnc_1D fnc_ptr; */

  int m;
  
  h_grid **child;

  if ( g == NULL || g->x == NULL || g->u == NULL )
      _STAT_MSG ( "Assign Cauchy data: using given initial conditions",
                  "grid non allocated",
                  ERROR, 0 );

  /* if ( g->is_master != H_TRUE  ) */
  /*     _STAT_MSG ( "Assign Cauchy data: using given initial conditions", */
  /*                 "passed grid is not master grid", */
  /*                 ERROR, 0 ); */

  if ( g->rank != f->rank )
      _STAT_MSG ( "Assign Cauchy data: using given initial conditions",
                  "ranks are not equal",
                  ERROR, 0 );


  _h_acd_to_one_grid( g, f );

  if ( g->children == NULL )
      printf("\n\n g->children == NULL \n\n");
  else
      printf("\n\n g->children != NULL \n\n");
  
  if ( g->children != NULL ) {
      child = (h_grid **) g->children;
  
      for (m = 0; m < g->Nchildren; m++) {
          _h_assign_cauchy_data ( child[m], p, f );
      }
  }
  


  /* /\* for ( i = 0; i < rank; i++ ) { *\/ */
  /* /\*     fnc_ptr = va_arg ( arguments, H_DBL (*)(H_DBL, void*) ); *\/ */
  /* /\*     if ( fnc_ptr == NULL ) { *\/ */
  /* /\*         va_end ( arguments ); *\/ */
  /* /\*         _STAT_MSG ( "Assign Cauchy data: using given initial conditions", *\/ */
  /* /\*                     "number of arguments is not equal to rank", *\/ */
  /* /\*                     ERROR, 0 ); *\/ */
  /* /\*     } *\/ */
  /* /\*     else { *\/ */

  /* N = g->N; */
  /* Lghost = g->Lghost; */
  /* Rghost = g->Rghost; */
  /* Ntotal = N+Lghost+Rghost; */
          
  /* /\* for (l = 0; l < lmax; l++) { /\\* loop for all grids hierarchy *\\/ *\/ */

  /* while ( l ) { */
  /*     _h_acd_to_one_grid( g, p, f ); */
      
  /*     for (i = 0; i < f->rank; i++) { */
  /*         fnc_ptr = f->C_da[i]; */

  /*         for (j = 0; j < Ntotal; j++) { */
  /*             g->u[i*Ntotal+j] = fnc_ptr( g->x[j], params ); */
  /*         } */
  /*     } */

  /* if( g_m->is_master == H_TRUE ) { */
  /*                 for (j = 0; j < Ntotal; j++) { */
  /*                     g->u[i*Ntotal+j] = fnc_ptr( g->x[j], params ); */
  /*                 } */
  /*             } */
  /*             else { */
  /*                 for ( m = 0, m < g->Nchild, m++ ) */
  /*                     g_c = (h_grid*) g_m->child[m]; */
                  
  /*                 for (j = 0; j < Ntotal; j++) { */
  /*                     g->u[i*Ntotal+j] = fnc_ptr( g->x[j], params ); */
  /*                 } */
                  
  /*             } */
  /*         } */
  /*     } */
      
  /*         g = cg; */
  /*     } */


  _STAT_MSG ( "Assign Cauchy data: using given initial conditions",
              NULL,
              OK, 0 );
}


/* stare funkcje */
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
