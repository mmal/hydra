#ifndef _LOG_H_
#define _LOG_H_


#include <stdlib.h>
#include <lwl.h>


typedef struct
{

  char *l_file;
  char *l_file_emerg;
  char *l_file_alert;
  char *l_file_crit;
  char *l_file_err;
  char *l_file_warning;
  char *l_file_notice;
  char *l_file_info;
  char *l_file_debug;

  lwlh_t h;                     /**< log handler */

} h_log;


h_log *h_alloc_log ( void );


#endif /* _LOG_H_ */
