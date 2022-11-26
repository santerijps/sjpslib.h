#ifndef SJPSLIB_PROCESS_H

  #define SJPSLIB_PROCESS_H

  #ifdef _WIN32

    #ifndef _INC_WINDOWS
      #include <windows.h>
    #endif

    #include "types.h"

    const string _PROCESS_CMD_PATH = "C:\\WINDOWS\\system32\\cmd.exe";
    const string _PROCESS_PS_PATH = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";

    struct Process {
      STARTUPINFO si;
      PROCESS_INFORMATION pi;
      u64 error;
    };

    typedef struct Process Process;

    /* Create a new process. The process needs to be started for it to run. */
    Process proc_create(string command) {
      Process p;
      ZeroMemory(&p, sizeof(Process));
      stringfmt(cmd, 1024, "%s /c %s", f_exists(_PROCESS_PS_PATH) ? _PROCESS_PS_PATH : _PROCESS_CMD_PATH, command);
      if(!CreateProcess(NULL, cmd, NULL, NULL, 0, CREATE_SUSPENDED, NULL, NULL, &p.si, &p.pi)) {
        p.error = GetLastError();
      }
      return p;
    }

    /* Create a new process and start it. */
    Process proc_spawn(string command) {
      Process p = proc_create(command);
      ResumeThread(p.pi.hThread);
      return p;
    }

    /* Start a process. */
    void proc_start(Process *p) {
      ResumeThread(p->pi.hThread);
    }

    /* Wait for a process to finish. */
    void proc_join(Process *p) {
      WaitForSingleObject( p->pi.hProcess, INFINITE );
    }

    /* Kill a process. */
    void proc_kill(Process *p) {
      TerminateThread(p->pi.hThread, CTRL_C_EVENT);
      TerminateProcess(p->pi.hProcess, CTRL_C_EVENT);
      CloseHandle(p->pi.hThread);
      CloseHandle(p->pi.hProcess);
    }

  #endif

#endif