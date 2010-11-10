

#include "stat_msg.h"


lwlh_t h;


void h_init_log ( void )
{
  /* FILE  *file = fopen("file", "w"); */
  /* FILE  *fileINFO = fopen("info", "w"); */
  

  h = lwl_alloc ();

  lwl_set_attributes ( h,
                       LWL_TAG_OPTIONS, LWL_OPT_PRIORITY |
                       LWL_OPT_DATE | LWL_OPT_TIME |
                       LWL_OPT_PERROR | LWL_OPT_USE_LOCALE , LWL_TAG_DONE );

  
  /* lwl_set_attributes ( h, LWL_TAG_PREFIX, */
  /*                      "my example app", */
  /*                      LWL_TAG_FILE, file, */
  /*                      LWL_TAG_OPTIONS, LWL_OPT_PRIORITY | */
  /*                      LWL_OPT_DATE | LWL_OPT_TIME | LWL_OPT_PREFIX | */
  /*                      LWL_OPT_PERROR | LWL_OPT_USE_LOCALE , LWL_TAG_DONE ); */
  
  /* lwl_set_attributes ( h, LWL_TAG_FILE_INFO, fileINFO ); */
  
}


void h_free_log ( void )
{
  if ( h != NULL ) {
      lwl_free ( h );
      h = NULL;
  }
}

void STAT_MSG ( const char * msg, const char * rsn, const int stat_id,
                       const char * file, int line, const int errno )
{

  /* lwl_write_log ( h, LWL_PRI_INFO, "%s %s %i %s %i %i", */
  /*                 msg, rsn, stat_id, file, line, errno ); */

  lwl_write_log ( h, LWL_PRI_INFO, "%s", msg );

  return;
}

void STAT_MSG_old ( const char * msg, const char * rsn, const int stat_id,
                const char * file, int line, const int errno )
{
  int i;
  int stat_len;
  int n_sep;
  
  int msg_len = strlen ( msg );
  
  int col = atoi( getenv( "COLUMNS" ) );

  const int sep = 3+4;

  char ESC=27;
  char * TAB="    ";
  
  char * stat_ok = "[ OK ]\n";
  char * stat_fa = "[ FAILED ]\n";
  char * stat_wa = "[ WARNING ]\n";
  
  char * stat_msg;
            
  switch ( stat_id ) {
    case ERROR:
        stat_len = 6+4; /* FAILED */
        stat_msg = stat_fa;
        break;
    case OK:
        stat_len = 2+4; /* OK */
        stat_msg = stat_ok;
        break;
    case WARNING:
        stat_len = 7+4; /* WARNING */
        stat_msg = stat_wa;  
        break;
    default:
        printf( " ERROR: message status not recognized.\n");
        exit(1);
        break;
  }

  if ( col >= msg_len+sep+stat_len ) {
      n_sep = col - msg_len - stat_len-4;
      printf( " * %s", msg );
      for (i = 0; i < n_sep; i++) {
          printf(" ");
      }
          printf( "%s", stat_msg );
  }
  else {
      n_sep = col - stat_len-4;
      printf( " * %s\n", msg );
      for (i = 0; i < n_sep; i++) {
          printf(" ");
      }
      printf( " %s", stat_msg );
  }

  /* If given a reason for the message, print it. */
  if ( rsn != NULL ) {
      if ( stat_id == 0 ) {
          printf("%c[1m",ESC);  /*- turn on bold */
          printf( "    ERROR: ");
          printf("%c[0m",ESC); /* turn off bold */
          printf("%s\n", rsn );
          printf( "%s" "in file %s on line %d\n" "%s" "Error no. %d\n",
                  TAB, file, line, TAB, errno);
      }
      else if ( stat_id == 2 ) {
          printf( "    WARNING: %s\n", rsn );
      }
      else printf( "    %s\n", rsn );
  }
}
