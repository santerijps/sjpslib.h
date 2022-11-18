#ifndef SJPSLIB_TIMESTAMP_H

  #define SJPSLIB_TIMESTAMP_H

  #ifndef _SYS_TIME_H_
    #include <sys/time.h>
  #endif

  #ifndef _INC_STDIO
    #include <stdio.h>
  #endif

  /*
  Measure the time it takes to run a block of code.
  Where:
  - var is the name
  - format is a string to use in printing
  The format argument should contain a %llu to display the elapsed seconds.
  */
  #define timeit(var, body)           \
    u64 var = ts_now();\
    body;\
    var = ts_now() - var;

  /*
  Measure the time it takes to run a block of code.
  Where:
  - elapsed_var is the name of the u64 variable to contain the elapsed microseconds
  - body is a block of code
  - format is a string to use in printing
  */
  #define timeitf(body, elapsed_var, format, ...)\
    {\
      u64 elapsed_var;\
      unsigned long long __start = ts_now();\
      body;\
      elapsed_var = ts_now() - __start;\
      printf(format, __VA_ARGS__);\
    }

  /*
  Returns the current timestamp in microseconds.
  */
  unsigned long long ts_now()
  {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000000 + t.tv_usec;
  }

  /*
  Returns the current timestamp in seconds.
  */
  unsigned long long ts_now_s()
  {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec;
  }

  /*
  Returns the current timestamp in milliseconds.
  */
  unsigned long long ts_now_ms()
  {
    return ts_now() / 1000;
  }

#endif