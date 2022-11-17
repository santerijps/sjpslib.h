#ifndef SJPSLIB_MATH_H

  #define SJPSLIB_MATH_H

  #ifndef _MATH_H_
    #include <math.h>
  #endif

  // Returns the smallest value between a and b.
  #define min(a, b) ((a) < (b) ? (a) : (b))

  // Returns the largest value between a and b.
  #define max(a, b) ((a) > (b) ? (a) : (b))

  // Returns true if x is even.
  #define is_even(x) ((x) % 2 == 0)

  // Returns true if x is odd.
  #define is_odd(x) ((x) % 2 != 0)

  // Divides x with y, mutating x and returns x modulus y.
  unsigned long long divmod(unsigned long long *x, unsigned long long y)
  {
    unsigned long long m = (*x) mod y;
    *x /= y;
    return m;
  }

#endif