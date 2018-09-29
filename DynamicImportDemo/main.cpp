#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

#define MIN_VALID_ARGC 5
#define ARGC_WHEN_DUMP_BEFORE (MIN_VALID_ARGC + 1)
#define ARGC_WHEN_DUMP_AFTER (ARGC_WHEN_DUMP_BEFORE + 1)

#define TEXTMSG_USAGE "Usage: <dll_filename> <process_name> <string_to_replace> <new_string> "\
                      "[<dump_before_replace_filename>] [<dump_after_replace_filename>]"
#define TEXTMSG_DONE "Done."

#define E_PROCESS_NOT_FOUND "Process could not be found"
#define E_CANT_ACCESS_PROCESS "Can not get access to the specified process"
#define E_LIBRARY_NOT_LOADED "Dynamic library could not be loaded"
#define E_FUNCTION_NOT_FOUND "Required function could not be found in library"

#define FUNCNAME_DUMP_MEM "?DumpMem@@YGHKPBD@Z"
#define FUNCNAME_REPLACE_STRING "?ReplaceString@@YGHKPBD0@Z"

typedef INT WINAPI TDumpMem(DWORD, PCSTR);
typedef INT WINAPI TReplaceString(DWORD, PCSTR, PCSTR);

VOID Notify(PCSTR message);
DWORD GetPidByName(PCSTR pszName);
INT DumpMemByRelativePath(TDumpMem fnDumpMem, DWORD dwPid, PCSTR pszDumpFilePath);

INT main(INT argc, PCHAR argv[])
{
    if (argc < MIN_VALID_ARGC) {
        Notify(TEXTMSG_USAGE);
        return 1;
    }

    HMODULE hVMemStringReplacerDll = LoadLibrary(argv[1]);
    if (!hVMemStringReplacerDll) {
        Notify(E_LIBRARY_NOT_LOADED);
        return 1;
    }

    DWORD dwPid = GetPidByName(argv[2]);
    if (!dwPid) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }

    TDumpMem *fnDumpMem = (TDumpMem *)GetProcAddress(hVMemStringReplacerDll, FUNCNAME_DUMP_MEM);
    if (!fnDumpMem) {
        Notify(E_FUNCTION_NOT_FOUND);
        return 1;
    }
    TReplaceString *fnReplaceString = (TReplaceString *)GetProcAddress(hVMemStringReplacerDll, FUNCNAME_REPLACE_STRING);
    if (!fnReplaceString) {
        Notify(E_FUNCTION_NOT_FOUND);
        return 1;
    }

    if (argc >= ARGC_WHEN_DUMP_BEFORE && DumpMemByRelativePath(fnDumpMem, dwPid, argv[5])) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }
    if (fnReplaceString(dwPid, argv[3], argv[4])) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }
    if (argc >= ARGC_WHEN_DUMP_AFTER && DumpMemByRelativePath(fnDumpMem, dwPid, argv[6])) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }

    FreeLibrary(hVMemStringReplacerDll);

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

INT DumpMemByRelativePath(TDumpMem fnDumpMem, DWORD dwPid, PCSTR pszDumpFilePath)
{
    CHAR szDumpFileFullPath[MAX_PATH];
    GetFullPathNameA(pszDumpFilePath, MAX_PATH, szDumpFileFullPath, NULL);
    return fnDumpMem(dwPid, szDumpFileFullPath);
}