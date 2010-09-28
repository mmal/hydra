

#include "create_grid.h"


int asd=0;

/* void _h_create_child_grid ( h_grid *parent, h_grid **child, */
/*                             h_amrp *p, int m, int idL, int idR ) */
/* { */
/*   /\* ghost points, what with them ? *\/  */
/*   int i; */

/*   int rr = p->rr; /\* refinement ratio *\/ */

/*   int sp = p->ngh; /\* ngh number of ghosts points *\/ */

/*   int N_c = (idR-idL)*rr+1; /\* number of child grid points *\/  */
  
/*   int Lghost_c = 0, Rghost_c = 0; /\* left and right ghost points of the child grid *\/ */

/*   H_DBL h_p = parent->h; /\* spatial spacing of parent grid *\/ */
  
/*   H_DBL *x_c = h_get_grid_positions ( parent ); /\* TODO: there was mistake ! *\/ */

/*   H_DBL xL_c = x_c[idL]; /\* coordinate of the left end of the child grid *\/ */

/*   H_DBL xR_c = x_c[idR]; /\* coordinate of the right end of the child grid *\/ */

/*   /\* H_DBL xL_c = parent->x[idL]; /\\* coordinate of the left end of the child grid *\\/ *\/ */

/*   /\* H_DBL xR_c = parent->x[idR]; /\\* coordinate of the right end of the child grid *\\/ *\/ */

/*   H_DBL xL_m, xR_m; /\* coordinates of the ends of the master grid *\/ */
  
/*   h_grid *fine_grid = h_alloc_grid ( ); */

/*   h_grid *master_grid = h_alloc_grid ( ); */

/*   if ( parent->is_master == H_TRUE  ) { /\* parent grid is a master grid *\/ */

/*       xL_m = parent->xL; */
/*       xR_m = parent->xR; */
/*       fine_grid->master = parent; /\* this was *\/ */
/*       master_grid = (h_grid*) parent; /\* missing *\/ */

/*   } */
/*   else { /\* parent grid is not a master grid *\/ */

/*       master_grid = (h_grid *) parent->master; */
      
/*       /\* if( master_grid == NULL ) VL(("l=%d master_grid == NULL\n", parent->l)); *\/ */
/*       /\* else VL(("l=%d master_grid != NULL\n", parent->l)); *\/ */
      
/*       xL_m = master_grid->xL; */
/*       xR_m = master_grid->xR; */
/*   } */

  
/*   for (i = 1; i <= sp; i++) { */
/*       if (xL_c - i*h_p/rr > xL_m) */
/*           Lghost_c++; */
/*       if (xR_c + i*h_p/rr < xR_m) */
/*           Rghost_c++; */
/*   } */
  
/*   /\* VL(( "create_grid.c xL_m=%f, xR_m=%f\n", xL_m, xR_m )); *\/ */
/*   /\* VL(( "create_grid.c m=%d: Lghost=%d, Rghost=%d\n", m, Lghost_c, Rghost_c )); *\/ */
  

/*   /\* initializing fine grid *\/ */
/*   h_init_grid ( fine_grid, xL_c, xR_c, N_c, Lghost_c, Rghost_c, */
/*                 parent->rank, (parent->l)+1, m ); */

/*   /\* VL(("create_grid.c m=%d, fine_grid->xL=%f, fine_grid->xR=%f\n", *\/ */
/*   /\*     m, fine_grid->xL, fine_grid->xR )); *\/ */
  
/*   fine_grid->master = (void *) master_grid; */

/*   fine_grid->parent = (void *) parent; */

/*   *child = fine_grid; */
/* } */


/* void _h_create_offspring_grids ( h_grid *cg, h_amrp *p, */
/*                                 int *idL, int *idR, int Ngrids ) */
/* { */
/*   /\* h_grid *g = fg; *\/ */
/*   int m; */
  
/*   h_grid **child = (h_grid **) malloc( Ngrids*sizeof( h_grid *) ); */

/*   /\* CAUSED FREE ERROR *\/ */
/*   /\* for (m = 0; m < Ngrids; m++) { *\/ */
/*   /\*     child[m] = h_alloc_grid ( ); *\/ */
/*   /\* } *\/ */
/*   /\* VL(("create_grid.c _h_create_offspring_grids TEST\n")); *\/ */

/*   /\* if ( cg->l > 1 ) { *\/ */
/*   /\*     h_grid *t_Lsibling = &(cg->Lsibling); *\/ */
/*   /\*     int t_Nchildren = t_Lsibling->l;  *\/ */
/*   /\*     /\\* int t_m = t_Lsibling->children[t_Nchildren]; *\\/ *\/ */
/*   /\*     /\\* int t_m = ((h_grid*) ((h_grid*) cg->Lsibling)->children[((h_grid*) cg->Lsibling)->Nchildren-1])->m; *\\/ *\/ */
/*   /\*     VL((" *** t_Nchildren=%d\n", t_Lsibling->l)); *\/ */
/*   /\* } *\/ */

/*   for (m = 0; m < Ngrids; m++) { */
/*       /\* VL(("create_grid.c _h_create_offspring_grids m=%d TEST\n", m)); *\/ */
/*       child[m] = h_alloc_grid(); */
/*       _h_create_child_grid ( cg, &child[m], p, m, idL[m], idR[m] ); */
/*   } */

/*   /\* if ( cg->l > 1 ) { *\/ */
/*   /\*     h_grid *t_sibling = (h_grid*) &(cg->sibling); *\/ */
/*   /\*     child[Ngrids-1]->neighbour = t_sibling->children; *\/ */
/*   /\* } *\/ */
  
/*   /\* for (m = 0; m < Ngrids-1; m++) { *\/ */
/*   /\*     child[m]->sibling =  &child[m+1]; *\/ */
/*   /\*     child[m+1]->Lsibling = &child[m]; *\/ */
/*   /\* } *\/ */

/*   cg->children = (void **) child; */
/*   cg->Nchildren = Ngrids; */


/*   /\* if ( parent->is_master == H_TRUE  ) { /\\* parent grid is a master grid *\\/ *\/ */
/*   /\*     child[m] = parent->xL; *\/ */
/*   /\*     xR_m = parent->xR; *\/ */
/*   /\* } *\/ */
/*   /\* elseent->master != NULL ) { /\\* parent grid is not a master grid *\\/ *\/ */
/*   /\*     xL_m = parent->master->xL; *\/ */
/*   /\*     xR_m = parent->master->xR; *\/ */
/*   /\* } *\/ */
/* } */


/* void _h_create_set_of_grids ( h_hms *m ) */
/* { */
/*   int i, l=m->g->l, lmax = m->p->lmax; */
  
/*   int *id_fp; */

/*   int Nfp; */

/*   int *idL, *idR, Ngrids; */

/*   /\* h_grid *g_c; *\/ */

/*   /\* h_hms *m_c = (h_hms*) malloc( sizeof(h_hms*) ); *\/ */

/*   h_hms *m_c = h_alloc_hms( ); */
  
/*   /\* VL(("TEST1 l=%d, lmax=%d m=%d\n", l, lmax, mm)); *\/ */

/*   m->g->dt = (m->g->h)/(m->p->lmbd); /\* setting time step size *\/ */
  
/*   _h_acd_to_one_grid ( m->g, m->f ); */

/*   /\* VL(("TEST2 l=%d, lmax=%d m=%d\n", l, lmax, mm)); *\/ */

/*   if ( l < lmax ) { */
      
/*       h_flagging_points ( m, &id_fp, &Nfp ); */

/*       /\* VL(("TEST3 l=%d, lmax=%d m=%d\n", l, lmax, mm)); *\/ */

/*       /\* VL(("l=%d Nfp=%d", l, Nfp)); *\/ */
      
/*       if ( Nfp > 0 ) { */
      
/*           h_clustering_flagged ( id_fp, Nfp, m->p->buf, m->g->N, */
/*                                  &idL, &idR, &Ngrids ); */

/*           if( id_fp!=NULL ) */
/*               free( id_fp ); */
/*           /\* VL(("create_grid TEST\n")); /\\****\\/ *\/ */
/*           _h_create_offspring_grids ( m->g, m->p, idL, idR, Ngrids ); */
          
/*           if( idL!=NULL ) */
/*               free( idL ); */
/*           if( idR!=NULL ) */
/*               free( idR ); */

/*           for (i = 0; i < Ngrids; i++) { */
/*               /\* g_c = m->g->children[i]; *\/ */
/*               m_c->g = (h_grid*) m->g->children[i]; */
/*               m_c->p = m->p; */
/*               m_c->f = m->f; */
/*               /\* m->g = g_c; *\/  */
/*               _h_create_set_of_grids ( m_c ); */
/*           } */
/*       } */
/*       else { */
/*           _STAT_MSG ( "Create set of grids", */
/*                       "no flagged points at level l=", */
/*                       WARNING, 0 ); */
/*       } */
/*   } */
  
/*   VL(("asd=%d\n", asd++)); */
/*   /\* free ( m_c->f ); *\/ */
/*   free ( m_c ); */
  
/*   /\* h_free_hms ( m_c ); *\/ */
  
/*   /\* h_free_grid ( m_c->g ); *\/ */
/*   /\* h_free_amrp( m_c->p ); *\/ */
/*   /\* h_free_fnc( m_c->f ); *\/ */
/* } */





/* void _h_acd_to_grid ( h_grid *g, h_fnc *f ) */
/* { */
/*   char *fnc_msg = "Assign Cauchy data to grid"; */

/*   int i, j, Ntotal; */

/*   _fnc_1D fnc_ptr; */

/*   if ( g == NULL || g->x == NULL || g->u == NULL ) */
/*       _STAT_MSG ( fnc_msg, */
/*                   "h_grid is non initiated/unallocated", */
/*                   ERROR, 0 ); */
  
/*   if ( g->rank != f->rank ) */
/*       _STAT_MSG ( fnc_msg, */
/*                   "ranks of h_grid and h_fnc structures are not equal", */
/*                   ERROR, 0 ); */
  
/*   Ntotal = g->Ntotal; */
  
/*   /\* loop through initial functions *\/ */
/*   for (i = 0; i < f->rank; i++) {  */
      
/*       fnc_ptr = f->C_da[i]; */
      
/*       /\* loop through the grid points *\/ */
/*       for (j = 0; j < Ntotal; j++) {  */
/*           g->u[i*Ntotal+j] = fnc_ptr( g->x[j], f->params ); */
/*       } */
/*   } */
/* } */



/* void _h_acd_to_glevel ( h_glevel *glevel, h_fnc *fnc ) */
/* { */
/*   char *fnc_msg = "Assign Cauchy data to glevel"; */
  
/*   int m, M; */

/*   int Lghost, Rghost, Ntotal; */

/*   _fnc_1D fnc_ptr; */

/*   if ( glevel == NULL ) { */
/*       _STAT_MSG ( fnc_msg, */
/*                   "h_glevel is unallocated", */
/*                   WARNING, 0 ); */
/*   } */
/*   else if ( fnc == NULL ) { */
/*       _STAT_MSG ( fnc_msg, */
/*                   "h_fnc is unallocated", */
/*                   WARNING, 0 ); */
/*   } */
/*   else { */
/*       M = glevel->M; /\* # of allocated grids in glevel *\/ */

/*       for (m = 0; m < M; m++) { */
/*           _h_acd_to_grid ( glevel->grid[m], fnc ); */
/*       } */
/*   } */
/* } */



/* /\* void _h_assign_cauchy_data ( h_grid *g, h_amrp *p, h_fnc *f ) *\/ */
/* /\* { *\/ */
/* /\*   /\\* int i, j; *\\/ *\/ */
/* /\*   /\\* int l=0, N; *\\/ *\/ */

/* /\*   /\\* int lmax = p->lmax; *\\/ *\/ */

/* /\*   /\\* int Lghost, Rghost, Ntotal; *\\/ *\/ */

/* /\*   /\\* _fnc_1D fnc_ptr; *\\/ *\/ */

/* /\*   int m; *\/ */
  
/* /\*   h_grid **child; *\/ */

/* /\*   if ( g == NULL || g->x == NULL || g->u == NULL ) *\/ */
/* /\*       _STAT_MSG ( "Assign Cauchy data: using given initial conditions", *\/ */
/* /\*                   "grid non allocated", *\/ */
/* /\*                   ERROR, 0 ); *\/ */

/* /\*   /\\* if ( g->is_master != H_TRUE  ) *\\/ *\/ */
/* /\*   /\\*     _STAT_MSG ( "Assign Cauchy data: using given initial conditions", *\\/ *\/ */
/* /\*   /\\*                 "passed grid is not master grid", *\\/ *\/ */
/* /\*   /\\*                 ERROR, 0 ); *\\/ *\/ */

/* /\*   if ( g->rank != f->rank ) *\/ */
/* /\*       _STAT_MSG ( "Assign Cauchy data: using given initial conditions", *\/ */
/* /\*                   "ranks are not equal", *\/ */
/* /\*                   ERROR, 0 ); *\/ */


/* /\*   _h_acd_to_one_grid( g, f ); *\/ */

/* /\*   if ( g->children == NULL ) *\/ */
/* /\*       printf("\n\n g->children == NULL \n\n"); *\/ */
/* /\*   else *\/ */
/* /\*       printf("\n\n g->children != NULL \n\n"); *\/ */
  
/* /\*   if ( g->children != NULL ) { *\/ */
/* /\*       child = (h_grid **) g->children; *\/ */
  
/* /\*       for (m = 0; m < g->Nchildren; m++) { *\/ */
/* /\*           _h_assign_cauchy_data ( child[m], p, f ); *\/ */
/* /\*       } *\/ */
/* /\*   } *\/ */
  


/* /\*   /\\* /\\\* for ( i = 0; i < rank; i++ ) { *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*     fnc_ptr = va_arg ( arguments, H_DBL (*)(H_DBL, void*) ); *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*     if ( fnc_ptr == NULL ) { *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*         va_end ( arguments ); *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*         _STAT_MSG ( "Assign Cauchy data: using given initial conditions", *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*                     "number of arguments is not equal to rank", *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*                     ERROR, 0 ); *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*     } *\\\/ *\\/ *\/ */
/* /\*   /\\* /\\\*     else { *\\\/ *\\/ *\/ */

/* /\*   /\\* N = g->N; *\\/ *\/ */
/* /\*   /\\* Lghost = g->Lghost; *\\/ *\/ */
/* /\*   /\\* Rghost = g->Rghost; *\\/ *\/ */
/* /\*   /\\* Ntotal = N+Lghost+Rghost; *\\/ *\/ */
          
/* /\*   /\\* /\\\* for (l = 0; l < lmax; l++) { /\\\\* loop for all grids hierarchy *\\\\/ *\\\/ *\\/ *\/ */

/* /\*   /\\* while ( l ) { *\\/ *\/ */
/* /\*   /\\*     _h_acd_to_one_grid( g, p, f ); *\\/ *\/ */
      
/* /\*   /\\*     for (i = 0; i < f->rank; i++) { *\\/ *\/ */
/* /\*   /\\*         fnc_ptr = f->C_da[i]; *\\/ *\/ */

/* /\*   /\\*         for (j = 0; j < Ntotal; j++) { *\\/ *\/ */
/* /\*   /\\*             g->u[i*Ntotal+j] = fnc_ptr( g->x[j], params ); *\\/ *\/ */
/* /\*   /\\*         } *\\/ *\/ */
/* /\*   /\\*     } *\\/ *\/ */

/* /\*   /\\* if( g_m->is_master == H_TRUE ) { *\\/ *\/ */
/* /\*   /\\*                 for (j = 0; j < Ntotal; j++) { *\\/ *\/ */
/* /\*   /\\*                     g->u[i*Ntotal+j] = fnc_ptr( g->x[j], params ); *\\/ *\/ */
/* /\*   /\\*                 } *\\/ *\/ */
/* /\*   /\\*             } *\\/ *\/ */
/* /\*   /\\*             else { *\\/ *\/ */
/* /\*   /\\*                 for ( m = 0, m < g->Nchild, m++ ) *\\/ *\/ */
/* /\*   /\\*                     g_c = (h_grid*) g_m->child[m]; *\\/ *\/ */
                  
/* /\*   /\\*                 for (j = 0; j < Ntotal; j++) { *\\/ *\/ */
/* /\*   /\\*                     g->u[i*Ntotal+j] = fnc_ptr( g->x[j], params ); *\\/ *\/ */
/* /\*   /\\*                 } *\\/ *\/ */
                  
/* /\*   /\\*             } *\\/ *\/ */
/* /\*   /\\*         } *\\/ *\/ */
/* /\*   /\\*     } *\\/ *\/ */
      
/* /\*   /\\*         g = cg; *\\/ *\/ */
/* /\*   /\\*     } *\\/ *\/ */


/* /\*   _STAT_MSG ( "Assign Cauchy data: using given initial conditions", *\/ */
/* /\*               NULL, *\/ */
/* /\*               OK, 0 ); *\/ */
/* /\* } *\/ */


