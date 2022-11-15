#ifndef SJPSLIB_STRING_H

  #define SJPSLIB_STRING_H

  #include "types.h"

  #ifndef _INC_STRING
    #include <string.h>
  #endif

  // String equals. Returns true if equal.
  boolean s_eq(const string a, const string b)
  {
    return strcmp(a, b) == 0;
  }

#endif