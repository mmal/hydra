

#include "boialg.h"



void h_boialg ( h_hms *m )
{
  int i, status;

  int repeat = pow( m->p->rr, m->g->l );

  H_DBL t = m->g->t;
  H_DBL dt = m->g->dt;

  h_hms *m_c = h_alloc_hms( );

  
  do {
      repeat--;
      
      status = _h_step ( t, t+dt, dt, m->g->u, m ); 

      if ( status != GSL_SUCCESS ) {
          _STAT_MSG("BO integration algorithm",
                    "step status != GSL_SUCCESS",
                    H_WA, 0);
          break;
      }

      VL(("stepping for l=%d, m=%d, repeat=%d, dt=%e\n",
          m->g->l,
          m->g->m,
          repeat, dt));
      /* sleep ( 2 ); */
      
      for (i = 0; i < m->g->Nchildren; i++) {
          m_c->g = (h_grid*) m->g->children[i];
          m_c->p = m->p;
          m_c->f = m->f;
          h_boialg ( m_c );
      }
      m->g->t = t+dt;
      
      h_update ( m->g, m_c->g );
      
  } while ( repeat > 0 );
  
}
