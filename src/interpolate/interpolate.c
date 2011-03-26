
#include "src/interpolate/interpolate.h"

H_DBL *_h_compute_D2 (H_DBL xa[], H_DBL ya[], int n)
{
  int i;
  H_DBL h2, *y2a = (H_DBL*) malloc( n*sizeof(H_DBL) );

  h2=(xa[1]-xa[0])*(xa[1]-xa[0]);



  /* metoda 4'go rzedu  */

  y2a[0]=(35*ya[0] - 104*ya[1] + 114*ya[2] - 56*ya[3] + 11*ya[4])/(12*h2);

  y2a[1]=(11*ya[0] - 20*ya[1] + 6*ya[2] + 4*ya[3] -ya[4])/(12*h2);
  
  for (i = 2; i < n-2; i++)
      y2a[i]=-((ya[-2 + i] - 16*ya[-1 + i] + 30*ya[i] - 16*ya[1 + i] +ya[2 + i])/(12*h2));

  y2a[n-2]=-((ya[-5 + n] - 4*ya[-4 + n] - 6*ya[-3 + n] + 20*ya[-2 + n] - 11*ya[-1 + n])/(12*h2));

  y2a[n-1]=(11*ya[-5 + n] - 56*ya[-4 + n] + 114*ya[-3 + n] - 104*ya[-2 + n] + 35*ya[-1 + n])/(12*h2);

    
    
/*   /\* metoda 6'go rzedu *\/ */
/*   /\* 0 pkt *\/ */
/*   y2a[0]=(938*ya[0]-4014*ya[1+0]+7911*ya[2+0]-9490*ya[3+0]+7380*ya[4+0]-3618*ya[5+0]+1019*ya[6+0]-126*ya[7+0])/(180*h2); */
/*   /\* 1 pkt *\/ */
/*   y2a[1]=(126*ya[-1+1]-70*ya[1]-486*ya[1+1]+855*ya[2+1]-670*ya[3+1]+324*ya[4+1]-90*ya[5+1]+11*ya[6+1])/(180*h2); */
/*   /\* 2 pkt *\/ */
/*   y2a[2]=(-11*ya[-2+2]+214*ya[-1+2]-378*ya[2]+130*ya[1+2]+85*ya[2+2]-54*ya[3+2]+16*ya[4+2]-2*ya[5+2])/(180*h2); */
/* /\* #pragma omp for private(i) *\/ */
/*   for (i = 3; i < n-3; i++) { */
/*       y2a[i]=(2*ya[-3+i]-27*ya[-2+i]+270*ya[-1+i]-490*ya[i]+270*ya[1+i]-27*ya[2+i]+2*ya[3+i])/(180*h2); */
/*   } */
/*   /\* n-3 pkt *\/ */
/*   y2a[n-3]=(-2*ya[-5+n-3]+16*ya[-4+n-3]-54*ya[-3+n-3]+85*ya[-2+n-3]+130*ya[-1+n-3]-378*ya[n-3]+214*ya[1+n-3]-11*ya[2+n-3])/(180*h2); */
/*   /\* n-2 pkt *\/ */
/*   y2a[n-2]=(11*ya[-6+n-2]-90*ya[-5+n-2]+324*ya[-4+n-2]-670*ya[-3+n-2]+855*ya[-2+n-2]-486*ya[-1+n-2]-70*ya[n-2]+126*ya[1+n-2])/(180*h2); */
/*   /\* n-1 pkt *\/ */
/*   y2a[n-1]=(-126*ya[-7+n-1]+1019*ya[-6+n-1]-3618*ya[-5+n-1]+7380*ya[-4+n-1]-9490*ya[-3+n-1]+7911*ya[-2+n-1]-4014*ya[-1+n-1]+938*ya[n-1])/(180*h2); */
  
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
  *y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
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
