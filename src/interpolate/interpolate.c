
#include "src/interpolate/interpolate.h"

H_DBL *_h_compute_D2 (H_DBL xa[], H_DBL ya[], int n)
{
  int i;
  H_DBL h, *y2a = (H_DBL*) malloc( n*sizeof(H_DBL) );

  h=xa[1]-xa[0];
  
  for (i = 0; i < n; i++) {
      y2a[i] = fda_D2_eon_3 ( ya, h, n, i );
  }
  return y2a;
}

void _h_splint (H_DBL xa[], H_DBL ya[],
                H_DBL y2a[],
                int n,
                H_DBL x,
                H_DBL *y )
{
  int klo, khi, k;
  H_DBL h, b,a;

  klo=0;
  khi=n-1;
  while (khi-klo >1)
    {
        k=(khi+klo) >> 1;
        if ( xa[k] > x ) khi=k;
        else klo=k;
    }
  h=xa[khi]-xa[klo];
  a=(xa[khi]-x)/h;
  b=(x-xa[klo])/h;
  *y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]
                          +(b*b*b-b)*y2a[khi])*(h*h)/6.0;
}




void _h_interpolate_from_parent_to_child_grid ( h_grid *parent, h_grid *child )
{
  char *fnc_msg = "Interpolate from parent to child grid";
  
  int i, r;
  
  int rank = child->rank;
  int Nchild = child->Ntotal;
  int Nparent = parent->Ntotal;

  H_DBL *xchild;
  H_DBL *xparent;

  H_DBL *uchild, *uparent, *u2parent;


  if ( parent == NULL )
      _STAT_MSG ( fnc_msg,
                  "parent h_glevel is unallocated",
                  WARNING, 0 );
  else if ( child == NULL )
      _STAT_MSG ( fnc_msg,
                  "child h_glevel is unallocated",
                  WARNING, 0 );
  else {

      xchild = h_get_grid_positions_wghosts ( child );
      xparent = h_get_grid_positions_wghosts ( parent );

      if ( xparent == NULL )
          _STAT_MSG ( fnc_msg,
                      "parent x positions is unallocated",
                      WARNING, 0 );
      else if ( xchild == NULL )
          _STAT_MSG ( fnc_msg,
                      "child x positions is unallocated",
                      WARNING, 0 );
      else {
          /* interpolujemy r-ty rząd */
          for (r = 0; r < rank; r++)
            {
                uchild = h_get_grid_values_wghosts ( child, r );
                uparent = h_get_grid_values_wghosts ( parent, r );

                /* licze 2 pochodna przestrzenna funkcji dla
                 * siatki parent */ 
                u2parent = _h_compute_D2 (xparent, uparent, Nparent);

                /* interpoluje wszystkie punkty siatki child */
                for (i = 0; i < Nchild; i++) {

                    _h_splint (xparent, uparent,
                               u2parent,
                               Nparent,
                               xchild[i],
                               &uchild[i]);
                }

                /* zwalniam pamiec zaalokowana dla drugich pochodnych
                 * przestrzennych u2parent */
                if ( u2parent != NULL ) {
                    free ( u2parent );
                }
            }

      }
  }
}
