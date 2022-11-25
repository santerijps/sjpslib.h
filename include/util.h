#ifndef SJPSLIB_UTIL_H

  #define SJPSLIB_UTIL_H

  #ifndef _INC_STDLIB
    #include <stdlib.h>
  #endif

  #include "types.h"

  /* Cycles unsigned integers from 0 to max (inclusive). */
  u64 cycle(u64 _max) {
    static u64 x = 0;
    if (x > _max) x = 0;
    return x++;
  }

  /* Read user input line. */
  string read_input(u64 buf_size) {
    string input = calloc(buf_size, sizeof(char));
    gets(input);
    return input;
  }

#endif