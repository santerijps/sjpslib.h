#ifndef SJPSLIB_STD_H

  #define SJPSLIB_STD_H

  #ifndef _INC_STDIO
    #include <stdio.h>
  #endif

  #ifndef __ASSERT_H_
    #include <assert.h>
  #endif

  #ifdef _WIN32
    #ifndef _INC_WINDOWS
      #include <windows.h>
    #endif
  #endif

  #include "include/argparser.h"
  #include "include/channel.h"
  #include "include/elegance.h"
  #include "include/fs.h"
  #include "include/list.h"
  #include "include/log.h"
  #include "include/math.h"
  #include "include/os.h"
  #include "include/process.h"
  #include "include/random.h"
  #include "include/string.h"
  #include "include/thread.h"
  #include "include/timestamp.h"
  #include "include/types.h"
  #include "include/util.h"

#endif
