#ifndef SJPSLIB_PROCESS_H

  #define SJPSLIB_PROCESS_H

  #ifdef _WIN32

    #ifndef _INC_WINDOWS
      #include <windows.h>
    #endif

    #ifndef _INC_TOOLHELP32
      #include <tlhelp32.h>
    #endif

    #include "types.h"

    void terminate_process_tree(DWORD pid) {
      PROCESSENTRY32 pe;
      memset(&pe, 0, sizeof(PROCESSENTRY32));
      pe.dwSize = sizeof(PROCESSENTRY32);
      HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
      if (Process32First(hSnap, &pe)) {
        BOOL bContinue = TRUE;
        while (bContinue) {
          if (pe.th32ParentProcessID == pid) {
            HANDLE hChildProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
            if (hChildProc) {
              TerminateProcess(hChildProc, 1);
              CloseHandle(hChildProc);
            }
          }
          bContinue = Process32Next(hSnap, &pe);
        }
      }
    }

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
      ZeroMemory(&p, sizeof(p));
      ZeroMemory(&p.si, sizeof(p.si));
      ZeroMemory(&p.pi, sizeof(p.pi));
      p.si.cb = sizeof(p.si);
      const string shell = f_exists(_PROCESS_PS_PATH) ? _PROCESS_PS_PATH : _PROCESS_CMD_PATH;
      strfmt(cmd, 1024, "%s /c %s", shell, command);
      if(!CreateProcess(shell, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED | CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &p.si, &p.pi)) {
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
      WaitForSingleObject(p->pi.hProcess, INFINITE);
    }

    /* Kill a process. */
    void proc_kill(Process *p) {
      terminate_process_tree(p->pi.dwProcessId);
      TerminateThread(p->pi.hThread, CTRL_C_EVENT);
      TerminateProcess(p->pi.hProcess, CTRL_C_EVENT);
      CloseHandle(p->pi.hThread);
      CloseHandle(p->pi.hProcess);
    }

  #endif

#endif