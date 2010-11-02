#ifndef _H_UTILS_H_
#define _H_UTILS_H_


#include "src/common/types.h"
#include "src/stat_log/stat_msg.h"

#include "src/stat_log/log.h"
#include "src/backup_and_save/backup.h"


typedef struct
{
  h_bas  *bas;

  h_log  *log;

  char   *run_name;

  char   *run_descr;
  
  char   *base_dir;

} h_utils;



h_utils * h_alloc_utils ( void );

void h_init_utils ( h_utils *u );

void h_free_utils ( h_utils *u );


#endif /* _H_UTILS_H_ */
