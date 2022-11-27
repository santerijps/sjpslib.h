#ifndef SJPSLIB_OS_H

  #define SJPSLIB_OS_H

  #ifndef __ASSERT_H_
    #include <assert.h>
  #endif

  #ifndef _INC_STDIO
    #include <stdio.h>
  #endif

  #include "fs.h"

  #ifdef _WIN32

    #ifndef _INC_WINDOWS
      #include <windows.h>
    #endif

    #ifndef _INC_TOOLHELP32
      #include <tlhelp32.h>
    #endif

    #include "types.h"

    void sleep(u64 ms) {
      Sleep(ms);
    }

    typedef struct FileInfo {
      string path;
      u64 size;
      u64 created;
      u64 accessed;
      u64 written;
    } FileInfo;

    FileInfo FileInfo_create(char *path, u64 size, u64 created, u64 accessed, u64 written) {
      FileInfo fi;
      fi.path = (char*) malloc(sizeof(char) * 2048);
      strcpy(fi.path, path);
      fi.size = size;
      fi.created = created;
      fi.accessed = accessed;
      fi.written = written;
      return fi;
    }

    typedef struct FileInfoList {
      u64 cap;
      u64 len;
      FileInfo *items;
    } FileInfoList;

    /* Create a new FileInfoList. */
    FileInfoList FileInfoList_create(u64 cap) {
      FileInfoList l = { cap, 0, (FileInfo*) malloc(sizeof(FileInfo) * cap) };
      return l;
    }

    /*
    Initialize (allocate memory) a FileInfoList.
    Remember to free the memory with FileInfoList_free().
    */
    void FileInfoList_init(FileInfoList *fil, u64 cap) {
      assert(fil != NULL);
      fil->cap = cap;
      fil->len = 0;
      fil->items = (FileInfo*) malloc(sizeof(FileInfo) * cap);
    }

    /* Free the memory used by the FileInfoList */
    void FileInfoList_free(FileInfoList *fil) {
      assert(fil != NULL);
      for (u64 i = 0; i < fil->len; i++) {
        free(fil->items[i].path);
      }
      free(fil->items);
      fil->items = NULL;
    }

    /* Append a FileInfo to the FileInfoList. */
    void FileInfoList_append(FileInfoList *fil, FileInfo fi) {
      assert(fil != NULL);
      fil->items[fil->len++] = fi;
      if (fil->len == fil->cap) {
        fil->items = (FileInfo*) realloc(fil->items, (fil->cap *= 2));
      }
    }

    /* Retrieve a FileInfo from the FileInfoList at the specified index. */
    FileInfo FileInfoList_get(FileInfoList *fil, u64 index) {
      assert(fil != NULL);
      assert(index < fil->len);
      return fil->items[index];
    }

    /*
    RecursivelylList files in a directory.
    The files are added to the FileInfoList structure.
    Remember to free the FileInfoList!

    TODO: Use wide strings instead!
    */
    bool list_files(const char *dir, FileInfoList *fil) {
      WIN32_FIND_DATA fdFile;
      HANDLE hFind = NULL;

      char path[2048] = {0};
      sprintf(path, "%s/*.*", dir);
      if((hFind = FindFirstFile(path, &fdFile)) == INVALID_HANDLE_VALUE) {
        return false;
      }

      do {
        if(strcmp(fdFile.cFileName, ".") && strcmp(fdFile.cFileName, "..") && fdFile.cFileName[0] != '.') {
          sprintf(path, "%s%c%s", dir, FILE_PATH_DELIM, fdFile.cFileName);
          if(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            list_files((char*)path, fil);
          } else{
            FileInfo fi = FileInfo_create(
              path,
              fdFile.nFileSizeLow,
              fdFile.ftCreationTime.dwLowDateTime,
              fdFile.ftLastAccessTime.dwLowDateTime,
              fdFile.ftLastWriteTime.dwLowDateTime
            );
            FileInfoList_append(fil, fi);
          }
        }
      } while(FindNextFile(hFind, &fdFile));
      FindClose(hFind);
      return true;
    }

  #endif

#endif