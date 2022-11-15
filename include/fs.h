#ifndef SJPSLIB_FS_H

  #define SJPSLIB_FS_H

  #ifndef _INC_STDIO
    #include <stdio.h>
  #endif

  #ifndef _INC_STDLIB
    #include <stdlib.h>
  #endif

  #ifndef _INC_STAT
    #include <sys/stat.h>
  #endif

  #ifndef _INC_FCNTL
    #include <fcntl.h>
  #endif

  #ifndef __ASSERT_H_
    #include <assert.h>
  #endif

  #include "types.h"

  // Returns true if the specified file path exists.
  bool f_exists(const string file_path)
  {
    return access(file_path, F_OK) == 0;
  }

  // Returns true if the file path points to a file.
  bool f_isfile(const string file_path)
  {
    struct stat s;
    return stat(file_path, &s) == 0 ? S_ISREG(s.st_mode) : false;
  }

  // Returns true if the file path points to a directory.
  bool f_isdir(const string file_path)
  {
    struct stat s;
    return stat(file_path, &s) == 0 ? S_ISDIR(s.st_mode) : false;
  }

  // Creates a file.
  void f_create(const string file_path)
  {
    i32 file_handle = open(file_path, O_CREAT);
    if (file_handle)
    {
      close(file_handle);
    }
  }

  // Removes the file in the specified file path.
  // Returns true if file was deleted.
  bool f_remove(const string file_path)
  {
    return remove(file_path) == 0;
  }

  // Returns the size of the specified file path in bytes.
  u64 f_size(const string file_path)
  {
    struct stat s;
    return !stat(file_path, &s) ? s.st_size : 0;
  }

  // Returns the contents of the specified file path as a string.
  string f_read_string(const string file_path)
  {
    u64 file_size = f_size(file_path);
    FILE ptr file = fopen64(file_path, "rb");
    string result = (string) malloc(sizeof(char) * (file_size + 1));
    result[file_size] = '\0';
    if (file != nullptr)
    {
      u64 read_bytes = fread(result, sizeof(char), file_size, file);
      assert(read_bytes == file_size);
      fclose(file);
    }
    return result;
  }

  // Returns the contents of the specified file path as a (struct) String.
  // String structs contain the text content (data) and size.
  String f_read_String(const string file_path)
  {
    FILE ptr file = fopen64(file_path, "rb");
    String result;
    result.size = f_size(file_path);;
    result.data = (string) malloc(sizeof(char) * (result.size + 1));
    result.data[result.size] = '\0';
    if (file != nullptr)
    {
      u64 read_bytes = fread(result.data, sizeof(char), result.size, file);
      assert(read_bytes == result.size);
      fclose(file);
    }
    return result;
  }

#endif