#ifndef SJPSLIB_ELEGANCE_H

  #define SJPSLIB_ELEGANCE_H

  #define mod %
  #define not !
  #define and &&
  #define or ||
  #define xor ^
  #define bitand &
  #define bitor |
  #define equals ==

  #define is =

  #define when if(
  #define then )
  #define done ;
  #define begin {
  #define end }

  // An infinite loop.
  #define loop while (1)

  // Creates a variable and increments it
  // until it reaches stop (exclusive).
  #define countup(i, start, stop) \
    for (i64 i = start; i < stop; i++)

  // Creates a variable and decrements it
  // until it reaches stop (exclusive).
  #define countdown(i, start, stop) \
    for (i64 i = start; i > stop; i--)

#endif