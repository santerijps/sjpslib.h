#ifndef SJPSLIB_THREAD_H

  #define SJPSLIB_THREAD_H

  #ifdef _WIN32

    #ifndef _INC_WINDOWS
      #include <windows.h>
    #endif

    struct Thread {
      DWORD id;
      HANDLE handle;
      void (*func)(void*);
      void *data;
    };

    typedef struct Thread Thread;

    DWORD WINAPI __thread_routine(void *thread_ptr) {
      struct Thread *thread = (struct Thread *) thread_ptr;
      if (thread->func != NULL) {
        thread->func(thread->data);
      }
      return 0;
    }

    /* Spawn a new thread. Returns a Thread struct. */
    struct Thread thread_spawn(void (*func)(void*), void *data) {

      static unsigned long index = 0;
      static struct Thread threads[1024] = {0};

      struct Thread t;
      t.func = func;
      t.data = data;

      t.handle = CreateThread(
        NULL,
        0,
        __thread_routine,
        &threads[index],
        0,
        &threads[index].id
      );

      threads[index++] = t;
      return t;
    }

    /* Kills a thread. */
    void thread_kill(struct Thread *t) {
      TerminateThread(t->handle, CTRL_C_EVENT);
      CloseHandle(t->handle);
    }

    /* Waits for a thread to finish. */
    void thread_join(struct Thread *t) {
      WaitForSingleObject(t->handle, INFINITE);
    }

  #endif

#endif