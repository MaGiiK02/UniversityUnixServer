#ifndef _MEMORY_HELPER_H_
#define _MEMORY_HELPER_H_

#include <stdio.h>

#define FREE(ptr_mh_macro) \
  if ((ptr_mh_macro) != NULL) {\
    free(ptr_mh_macro); \
    (ptr_mh_macro)=NULL; \
  }

#endif //_MEMORY_HELPER_H_
