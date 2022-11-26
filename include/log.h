#ifndef SJPSLIB_LOG_H

  #define SJPSLIB_LOG_H

  #ifndef _INC_STDIO
    #include <stdio.h>
  #endif

  #ifndef _INC_LOCALE
    #include <locale.h>
  #endif

  #ifndef _TIME_H_
    #include <time.h>
  #endif

  #define COLOR_RESET "\033[0m"
  #define COLOR_BOLD "\033[1m"
  #define COLOR_INFO "\033[36m"
  #define COLOR_WARN "\033[33m"
  #define COLOR_ERROR "\033[31m"

  #define println(...) (printf(__VA_ARGS__),puts(""))
  #define wprintln(...) (wprintf(__VA_ARGS__),puts(""))

  #define use_default_locale setlocale(LC_ALL, "")

  #define log_debugf(...)\
    fprintf(stderr, COLOR_BOLD "[%s :: ln:%d :: %s] " COLOR_RESET, __FILE__, __LINE__, __FUNCTION__);\
    fprintf(stderr, __VA_ARGS__)

  #define log_logf(t, ...){\
    time_t rt = time(NULL);\
    struct tm *lt = localtime(&rt);\
    fprintf(stderr, COLOR_BOLD "[%d-%02d-%02d %02d:%02d:%02d %s - %s] " COLOR_RESET,\
      lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, __FUNCTION__, t);\
    fprintf(stderr, __VA_ARGS__);}

  #define log_infof(...){\
    time_t rt = time(NULL);\
    struct tm *lt = localtime(&rt);\
    fprintf(stderr, COLOR_INFO "[%d-%02d-%02d %02d:%02d:%02d %s - INFO] " COLOR_RESET,\
      lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, __FUNCTION__);\
    fprintf(stderr, __VA_ARGS__);}

  #define log_warnf(...){\
    time_t rt = time(NULL);\
    struct tm *lt = localtime(&rt);\
    fprintf(stderr, COLOR_WARN "[%d-%02d-%02d %02d:%02d:%02d %s - WARN] " COLOR_RESET,\
      lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, __FUNCTION__);\
    fprintf(stderr, __VA_ARGS__);}

  #define log_errorf(...){\
    time_t rt = time(NULL);\
    struct tm *lt = localtime(&rt);\
    fprintf(stderr, COLOR_ERROR "[%d-%02d-%02d %02d:%02d:%02d %s - ERROR] " COLOR_RESET,\
      lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, __FUNCTION__);\
    fprintf(stderr, __VA_ARGS__);}

#endif
