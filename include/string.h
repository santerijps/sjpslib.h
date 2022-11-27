#ifndef SJPSLIB_STRING_H

  #define SJPSLIB_STRING_H

  #include "types.h"

  #ifndef _INC_STRING
    #include <string.h>
  #endif

  // String equals. Returns true if equal.
  boolean s_eq(const string a, const string b) {
    return strcmp(a, b) == 0;
  }

  /* If string a ends with string b. */
  boolean endswith(const string a, const string b) {
    u64 len_a = strlen(a);
    u64 len_b = strlen(b);
    return !strcmp(&a[len_a - len_b], b);
  }

  boolean startswith(const string a, const string b) {
    u64 len_b = strlen(b);
    string c = (string) calloc(len_b + 1, sizeof(char));
    for (u64 i = 0; i < len_b; i++) {
      c[i] = a[i];
    }
    boolean result = !strcmp(c, b);
    free(c);
    return result;
  }

#endif