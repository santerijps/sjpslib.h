#ifndef SJPSLIB_MATH_H

  #define SJPSLIB_MATH_H

  #ifndef _MATH_H_
    #include <math.h>
  #endif

  // Returns the smallest value between a and b.
  #define min(a, b) ((a) < (b) ? (a) : (b))

  // Returns the largest value between a and b.
  #define max(a, b) ((a) > (b) ? (a) : (b))

  #define is_even(x) ((x) % 2 == 0)
  #define is_odd(x) ((x) % 2 != 0)

#endif