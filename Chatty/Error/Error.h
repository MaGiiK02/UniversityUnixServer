#ifndef _ERROR_H_
#define _ERROR_H_

#include <errno.h>

static inline int is_error(int function_result){
  return function_result < 0;
}

static inline int is_no_error(int function_result){
  return !is_error(function_result);
}

#endif
