#ifndef SJPSLIB_STRING_H

  #define SJPSLIB_STRING_H

  #include "types.h"

  #ifndef _INC_STRING
    #include <string.h>
  #endif

  // String equals. Returns true if equal.
  boolean s_equals(const string a, const string b) {
    return strcmp(a, b) == 0;
  }

  /* If string a ends with string b. */
  boolean s_endswith(const string a, const string b) {
    u64 len_a = strlen(a);
    u64 len_b = strlen(b);
    return !strcmp(&a[len_a - len_b], b);
  }

  /* If string a starts with string b. */
  boolean s_startswith(const string a, const string b) {
    u64 len_b = strlen(b);
    string c = (string) calloc(len_b + 1, sizeof(char));
    for (u64 i = 0; i < len_b; i++) {
      c[i] = a[i];
    }
    boolean result = !strcmp(c, b);
    free(c);
    return result;
  }

  /*
  Returns the head part of a string with the first n chars.
  Performs an allocation, remember to free!
  */
  string s_lheada(const string s, u64 n) {
    string b = (string) calloc(n + 1, sizeof(char));
    for (u64 i = 0; i < n; i++) {
      b[i] = s[i];
    }
    return b;
  }

  /*
  Returns the head part of a string with the last n chars removed.
  Performs an allocation, remember to free!
  */
  string s_rheada(const string s, u64 n) {
    u64 len_b = strlen(s) - n;
    string b = (string) calloc(len_b + 1, sizeof(char));
    for (u64 i = 0; i < len_b; i++) {
      b[i] = s[i];
    }
    return b;
  }

  /* Creates a string variable with the head part of s with the first n chars. */
  #define s_lhead(name, s, n)\
    char name[n + 1];\
    {\
      for (u64 i = 0; i < n; i++)\
      {\
        name[i] = s[i];\
      }\
    }\

  /* Creates a string variable with the head part of s with the last n chars removed. */
  #define s_rhead(name, s, n)\
    char name[strlen(s) - n + 1];\
    {\
      for (u64 i = 0; i < (strlen(s) - n); i++)\
      {\
        name[i] = s[i];\
      }\
    }\

  /* Returns the tail part of a string without the first n chars. */
  #define s_ltail(s, n) (&(s)[(n)])

  /* Returns the tail part of a string including the last n chars. */
  #define s_rtail(s, n) (&(s)[strlen(s) - (n)])

#endif