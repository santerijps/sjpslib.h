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
  - fmt is a string
  - body is a block of code
  The fmt argument should contain a %llu to display the elapsed seconds.
  */
  #define timeit(fmt, body)           \
    {                                 \
      unsigned long long a = ts_now();\
      body;                           \
      printf(fmt, ts_now() - a);      \
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