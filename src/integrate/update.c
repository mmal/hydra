

#include "update.h"


int _h_update_grid ( h_grid *parent, h_grid *child  )
{
  VL((" *** Updating from child l=%d m=%d to parent l=%d m=%d\n",
      child->l, child->m, parent->l, parent->m ));
  
  /* należy znaleźć pozycję siatki gęstej na siatce rzadkiej */

  return H_OK;
}


int _h_update_glevel ( h_glevel *parent, h_glevel *child  )
{
  VL((" *** Updating from child glevel l=%d to parent glevel l=%d\n",
      child->l, parent->l ));
  
  /* należy znaleźć pozycję siatki gęstej na siatce rzadkiej */

  return H_OK;
}
