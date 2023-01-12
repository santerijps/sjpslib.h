#ifndef BITLIB_H
#define BITLIB_H

#ifndef _INC_STDIO
  #include <stdio.h>
#endif

#define BitSize(x) (sizeof(x) * 8)

#define SetBitLeft(x, n) (x) | (1 << sizeof(x) * 8 - (n))
#define SetBitRight(x, n) (x) | (1 << (n) - 1)

#define ClearBitLeft(x, n) (x) & ~(1 << sizeof(x) * 8 - (n))
#define ClearBitRight(x, n) (x) & ~(1 << (n) - 1)

#define ToggleBitLeft(x, n) (x) ^ (1 << sizeof(x) * 8 - (n))
#define ToggleBitRight(x, n) (x) ^ (1 << (n) - 1)

#define GetBitLeft(x, n) ((x) >> sizeof(x) * 8 - n) & 1
#define GetBitRight(x, n) (x) >> (n) - 1 & 1

/**
 * Print the bits of a number.
 * The number of bits will equal to the size of the data type.
 * @param x The number to convert to bits.
*/
#define PrintBits(x)\
{\
  unsigned long long i = 1ULL << sizeof(x) * 8 - 1;\
  do putchar((((x) & i) > 0) + 48); while (i >>= 1);\
  printf("\n");\
}

/**
 * Declare a new string variable that contains the bits.
 * The length of the string is equal to the number of bits.
 * @param id The name of the string variable.
 * @param x The number to convert to bits.
*/
#define BitString(id, x)\
char id[sizeof(x) * 8 + 1] = {0};\
{\
  unsigned long long i = 1ULL << sizeof(x) * 8 - 1, j = 0;\
  do id[j++] = (((x) & i) > 0) + 48; while (i >>= 1);\
}

#endif