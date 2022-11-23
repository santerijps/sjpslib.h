#ifndef SJPSLIB_ELEGANCE_H

  #define SJPSLIB_ELEGANCE_H

  // Elegance can be disabled by defining this before the import of std.
  #ifndef SJPSLIB_DISABLE_ELEGANCE

    #define noop ;

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

    // Loop n times.
    #define repeat(n) for (u64 __i = 0; __i < (u64) n; __i++)

    // A basic foreach.
    #define foreach(type, item, array, size, body)\
      {\
        type item = array[0];\
        for(u64 __i = 0; __i < (u64) size; __i++, item=array[__i])\
        {\
          body;\
        }\
      }

    // Creates a variable and increments it until it reaches stop (exclusive).
    // Similar to Python's range()
    #define countup(i, start, stop) \
      for (i64 i = (i64) start; i < (i64) stop; i++)

    // Creates a variable and decrements it until it reaches stop (exclusive).
    // Similar to Python's range()
    #define countdown(i, start, stop) \
      for (i64 i = (i64) start; i > (i64) stop; i--)

    /*
    Define a file context manager, similar to Python's context managers.
    This macro opens a file and closes it afterwards.
    If the file opening succeeds, the on_success_body will be run.
    Otherwise the on_error_body will be run.
    */
    #define filecontext(file_name, mode, file_var_name, on_success_body, on_error_body)\
      {\
        FILE *file_var_name = fopen64(file_name, mode);\
        if (file_var_name != NULL)\
        {\
          on_success_body;\
          fclose(file_var_name);\
        }\
        else \
          on_error_body;\
      }

    /*
    Define an allocation context for heep allocations.
    The allocation is free'd afterwards.
    */
    #define malloccontext(type, name, size, on_success_body, on_error_body)\
    {\
      type *name = (type*) malloc(sizeof(type) * size);\
      if (name != NULL) { memset(name, 0, size); on_success_body; free(name); }\
      else on_error_body;\
    }

    static void *__nullcheck_return_value;
    /*
    Call a function that returns a pointer. Run the code block in case of NULL.
    NOTE! Not thread safe.
    */
    #define nullcheck(func_call, on_null_return_body)\
      (__nullcheck_return_value = func_call, __nullcheck_return_value);\
      if (__nullcheck_return_value == NULL)\
      {\
        on_null_return_body\
      }

    /* Return from a function early. Execute body first. */
    #define return$(body) {body;return;}

    /* Break from a loop early. Execute the body first. */
    #define break$(body) {body; break;}

    /* Break if the condition is true. */
    #define breakloop(condition) if (condition) break

    /* Read a text file one buffer size at a time. */
    #define BufferedFileReader(file_name, buf_size, buf_name, body, on_error_body)\
    {\
      char buf_name[buf_size + 1] = {0};\
      FILE *_f = fopen64(file_name, "rb");\
      if (_f == NULL) { on_error_body; }\
      else while (fread(buf_name, sizeof(char), buf_size, _f) == buf_size)\
      {\
        body;\
      }\
      fclose(_f);\
    }\

    /* A switch-like macro for strings. */
    #define matcher(x, body) do {char *__s = x; body;} while (0)

    /* A case like macro for matching string in a matcher. */
    #define match(x) if (!strcmp(x, __s))

    char __matcher_matched;
    char *__matcher_s;

    /* Experimental switch-like macro for strings. NOT THREAD SAFE. */
    #define xmatcher(x) __matcher_s = x; __matcher_matched = 0;

    /* Experimental case like macro for matching string in a matcher. */
    #define xmatch(x) if (!__matcher_matched && !strcmp(x, __matcher_s) && (__matcher_matched = 1))

    /* Experimental default like macro for matching string in a matcher. */
    #define xnomatch if (!__matcher_matched)

  #endif

#endif