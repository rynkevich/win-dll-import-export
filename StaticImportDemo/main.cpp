#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include "main.h"

#define MIN_VALID_ARGC 4
#define ARGC_WHEN_DUMP_BEFORE (MIN_VALID_ARGC + 1)
#define ARGC_WHEN_DUMP_AFTER (ARGC_WHEN_DUMP_BEFORE + 1)

#define TEXTMSG_USAGE "Usage: <process_name> <string_to_replace> <new_string> "\
                      "[<dump_before_replace_filename>] [<dump_after_replace_filename>]"
#define TEXTMSG_DONE "Done."

#define E_PROCESS_NOT_FOUND "Process could not be found"
#define E_CANT_ACCESS_PROCESS "Specified process could not be accessed"

VOID Notify(PCSTR message);
DWORD GetPidByName(PCSTR pszName);
INT DumpMemByRelativePath(DWORD dwPid, PCSTR pszDumpRelativeFilePath);

INT main(INT argc, PCHAR argv[])
{
    if (argc < MIN_VALID_ARGC) {
        Notify(TEXTMSG_USAGE);
        return 1;
    }
    
    DWORD dwPid = GetPidByName(argv[1]);
    if (!dwPid) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }

    if (argc >= ARGC_WHEN_DUMP_BEFORE && DumpMemByRelativePath(dwPid, argv[4])) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }
    if (ReplaceString(dwPid, argv[2], argv[3])) {
        Notify(E_CANT_ACCESS_PROCESS);
        return 1;
    } 
    if (argc >= ARGC_WHEN_DUMP_AFTER && DumpMemByRelativePath(dwPid, argv[5])) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }

    Notify(TEXTMSG_DONE);
    return 0;
}

VOID Notify(PCSTR message)
{
    puts(message);
    getchar();
}

DWORD GetPidByName(PCSTR pszName)
{
    DWORD dwPid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 procentry;
    ZeroMemory(&procentry, sizeof(procentry));
    procentry.dwSize = sizeof(procentry);

    BOOL isFound = FALSE;
    if (Process32First(hSnapshot, &procentry))
    {
        do
        {
            if (!strcmp(procentry.szExeFile, pszName))
            {
                dwPid = procentry.th32ProcessID;
                isFound = TRUE;
            }
        } while (Process32Next(hSnapshot, &procentry) && !isFound);
    }

    CloseHandle(hSnapshot);
    return dwPid;
}

INT DumpMemByRelativePath(DWORD dwPid, PCSTR pszDumpRelativeFilePath)
{
    CHAR szDumpFileFullPath[MAX_PATH];
    GetFullPathNameA(pszDumpRelativeFilePath, MAX_PATH, szDumpFileFullPath, NULL);
    return DumpMem(dwPid, szDumpFileFullPath);
}