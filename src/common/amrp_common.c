

#include "amrp_common.h"


h_amrp *h_alloc_amrp ( void )
{
  h_amrp * p = (h_amrp*) malloc ( sizeof( h_amrp ) );

  if ( p == NULL )
      _STAT_MSG ( "Allocating amrp",
                  "cannot allocate amrp",
                  ERROR, 0 );
  else
      _STAT_MSG ( "Allocating amrp",
                  NULL,
                  OK, 0 );
  return p;
}


void h_init_amrp ( h_amrp *p, int argc, char *argv[] )
{
  int nerrors;

  struct arg_lit  *help = arg_lit0("h","help","shows this output and exits");

  struct arg_int  *rr = arg_int1("r","rr,refratio","<n>","refinement ratio");

  struct arg_int  *buf = arg_int1("b","buf,bufor","<n>","bufor size");

  struct arg_int  *sp = arg_int1("p",NULL,"<n>","scheme points");

  struct arg_int  *lmax = arg_int1(NULL,"lmax,level_max","<n>","maximal refinement level");

  struct arg_int  *lmbd = arg_int1(NULL,"lambda","<n>",
                                   "Courant–Friedrichs–Lewy conditio, Courant Number");

  struct arg_dbl  *errt = arg_dbl1("e","error","<n>","error tolerance");

  struct arg_end  *end = arg_end(12);

  void *argtable[] = {help,rr,buf,sp,lmax,lmbd,errt,end};


  if ( p==NULL )
      _STAT_MSG ( "Initializing amrp",
                  "amrp was not allocated",
                  ERROR, 0 );
  
  if ( arg_nullcheck( argtable ) != 0 )
      printf("error: insufficient memory\n");

  nerrors = arg_parse ( argc, argv, argtable );

  if ( nerrors == 0 ) {
      p->rr = rr->ival[0];
      p->buf = buf->ival[0];
      p->sp = buf->ival[0];
      p->lmax = lmax->ival[0];
      p->lmbd = lmbd->ival[0];
      p->errt = errt->dval[0];
  }
  else {
      arg_print_errors ( stdout, end, argv[0] );
      printf("\n Try '%s -h, --help' for more information.\n", argv[0] );
      exit(0);
  }
  
  if ( help->count != 0 ) {
      printf(" Usage: %s", argv[0]);
      
      arg_print_syntaxv ( stdout, argtable, "\n" );
      
      printf("\n %-30s  %s\n", "Option", "Meaning");
      
      arg_print_glossary ( stdout, argtable, "  %-30s %s\n" );
  }
  
  arg_freetable ( argtable, sizeof(argtable)/sizeof(argtable[0]) );

}


void h_free_amrp ( h_amrp *p )
{
  if ( p != NULL ) {
      free( p );
      _STAT_MSG ( "Feeing amrp",
                  NULL,
                  OK, 0 );
  }
  
  else
      _STAT_MSG ( "Feeing amrp",
                  "amrp was not allocated",
                  WARNING, 0 );
}
