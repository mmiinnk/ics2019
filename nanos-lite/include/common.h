#ifndef __COMMON_H__
#define __COMMON_H__

/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE
//#define HAS_VME

#include <am.h>
#include <klib.h>
#include "debug.h"


typedef char bool;
#ifndef __ISA_AM_NATIVE__
typedef int ssize_t;
typedef unsigned int size_t;
#endif

#define true 1
#define false 0

#endif
