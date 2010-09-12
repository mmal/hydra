#ifndef _STAT_MSG_H_
#define _STAT_MSG_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS


enum STAT_ID {
    ERROR=0,
    OK,
    WARNING
};

void STAT_MSG ( const char * msg, const char * rsn,
                const int stat_id, const char * file,
                int line, const int errno );


#define _STAT_MSG(msg, rsn, stat_id, errno)                          \
    do {                                                             \
        STAT_MSG(msg,rsn,stat_id,__FILE__,__LINE__,errno);           \
        if ( stat_id == ERROR ) exit(0);                             \
    } while (0)

__END_DECLS

#endif /* _STAT_MSG_H_ */


