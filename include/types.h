#ifndef SJPSLIB_TYPES_H

  #define SJPSLIB_TYPES_H

  typedef char i8;
  typedef short i16;
  typedef int i32;
  typedef long long i64;

  typedef unsigned char u8;
  typedef unsigned char byte;
  typedef unsigned short u16;
  typedef unsigned int u32;
  typedef unsigned long long u64;

  typedef float f32;
  typedef double f64;
  typedef double decimal;

  typedef char* string;
  typedef struct String { u64 size; string data; } String;

  typedef u8 boolean;

  #define bool u8
  #define true ((u8)1)
  #define false ((u8)0)

  #define ptr *
  #define ref &
  #define deref *
  #define any void*
  #define nullptr NULL

  // Return the size of a type in bits
  #define bitsize(type) (sizeof(type) * 8)

#endif