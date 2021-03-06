/**
 * @file   amrp_common.c
 * @author Maciej Maliborski <maciej.maliborski@gmail.com>
 * @date   Mon Oct 11 22:00:44 2010
 * 
 * @brief  The h_amrp structure management functions.
 * 
 * 
 */


#include "amrp_common.h"



/** 
 * Allocates memory for a h_amrp structure.
 * 
 * @return pointer to the h_amrp structure
 */
h_amrp *h_alloc_amrp ( void )
{
  h_amrp * p = (h_amrp*) malloc ( sizeof( h_amrp ) );

  if ( p == NULL )
      _STAT_MSG ( "Allocating h_amrp",
                  "cannot allocate h_amrp",
                  WARNING, 0 );
  return p;
}



/** 
 * Initializes, previously allocated, h_amrp structure
 * from program's command-line arguments. 
 * 
 * 
 * @param p pointer to the h_amrp structure
 * @param argc argument count
 * @param argv  argument vector
 * @return  void
 */
void h_init_amrp ( h_amrp *p, int argc, char *argv[] )
{
  char *fnc_msg = "Initializing h_amrp structure";
  
  int i, nerrors, exit_stat=1;
  
  struct arg_lit  *help = arg_lit0("h","help",                    "shows this output and exits");
  struct arg_int  *rr = arg_int0("r","rr,refratio","<n>",          "refinement ratio (defaults to 2)");
  struct arg_int  *buf = arg_int0("b","buf,bufor","<n>",          "bufor size (defaults to 1)");
  struct arg_int  *sp = arg_int0("p",NULL,"<n>",                  "scheme points (defaults to 2)");
  struct arg_int  *lmax = arg_int0(NULL,"lmax,level_max","<n>",   "maximal refinement level (defaults to 1)");
  struct arg_int  *lmbd = arg_int0(NULL,"lambda","<n>",           "Courant–Friedrichs–Lewy condition, Courant Number (defaults to 4)");
  struct arg_dbl  *errt = arg_dbl0("e","error","<n>",             "error tolerance (defaults to 9.0)");
  struct arg_lit  *version = arg_lit0(NULL,"version",             "print version information and exit");
  struct arg_end  *end = arg_end(20);

  void *argtable[] = {help,rr,buf,sp,lmax,lmbd,errt,version,end};

  /* verify the argtable[] entries were allocated sucessfully */
  if (arg_nullcheck(argtable) != 0)
    {
        /* NULL entries were detected, some allocations must have failed */
        printf("insufficient memory\n");
        exit_stat=0;
        goto exit;
    }

  /* set default values to components of the h_amrp structure */
  
  /* set refinement ratio default value to 2 */
  for (i = 0; i < rr->hdr.maxcount; i++)
      rr->ival[i]=2;  
  /* set bufor size default value to 1 */
  for (i = 0; i < buf->hdr.maxcount; i++)
      buf->ival[i]=1;
  /* set scheme points default value to 2 */
  for (i = 0; i < sp->hdr.maxcount; i++)
      sp->ival[i]=2;
  /* set maximal refinement level default value to 1 */
  for (i = 0; i < lmax->hdr.maxcount; i++)
      lmax->ival[i]=1;
  /* set Courant number default value to 4 */
  for (i = 0; i < lmbd->hdr.maxcount; i++)
      lmbd->ival[i]=4;
  /* set error tolerance default value to 9.0 */
  for (i = 0; i < errt->hdr.maxcount; i++)
      errt->dval[i]=1.e-2;

  if ( p==NULL )
      _STAT_MSG ( fnc_msg,
                  "h_amrp is unallocated",
                  ERROR, 0 );
  
  /* Parse the command line as defined by argtable[] */
  nerrors = arg_parse(argc,argv,argtable);
  
  /* special case: '--help' takes precedence over error reporting */
  if (help->count > 0)
    {
        printf(" Usage: %s", argv[0]);
        arg_print_syntaxv ( stdout, argtable, "\n" );
        printf("\n %-30s  %s\n", "Option", "Meaning");
        arg_print_glossary ( stdout, argtable, "  %-30s %s\n" );
        exit_stat=0;
        goto exit;
    }

  /* TODO: rewrite help/version info */
  /* special case: '--version' takes precedence error reporting */
  if (version->count > 0)
    {
        printf("Program using the library\n");
        printf("libhydra version 0.1\n");
        printf("Copyright (C) September 2010\n");
        /* printf("'%s' program using default \"hydra\" lib command line argument parser.\n",argv[0]); */
        printf("\n");
        printf("Written by Maciej K. Maliborski\n");
        exit_stat=0;
        goto exit;
    }
  
  /* If the parser returned any errors then display them and exit */
  if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,argv[0]);
        printf("Try '%s --help' for more information.\n",argv[0]);
        exit_stat=0;
        goto exit;
    }
    
  /* command line arguments are successfully parsed at this point. */
  p->rr = rr->ival[0];
  p->buf = buf->ival[0];
  p->sp = sp->ival[0];
  p->lmax = lmax->ival[0];
  p->lmbd = lmbd->ival[0];
  p->errt = errt->dval[0];
  p->ngh = (p->sp)*(p->rr)*4; /* TODO: check/modify this */

  exit_stat=1;

  exit:
  /* deallocate each non-null entry in argtable[] */
  arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
  if ( exit_stat != 1 )
      exit( 0 );
}



/** 
 * Frees the memory space pointed by p
 * 
 * @param p pointer to the h_amrp structure
 */
void h_free_amrp ( h_amrp *p )
{
  char *fnc_msg = "Freeing h_amrp structure";

  if ( p != NULL ) {
      free( p );
      p = NULL;
  }
  else
      _STAT_MSG ( fnc_msg,
                  "amrp is unallocated",
                  WARNING, 0 );
}
