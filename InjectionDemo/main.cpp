#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

#define MIN_VALID_ARGC 3

#define TEXTMSG_USAGE "Usage: <dll_filename> <process_name>"
#define TEXTMSG_DONE "Done."

#define E_PROCESS_NOT_FOUND "Process could not be found"
#define E_LIBRARY_NOT_FOUND "Dynamic library could not be found"
#define E_LOAD_LIBRARY_FUNCTION_NOT_FOUND "LoadLibraryA function was not found inside kernel32.dll"
#define E_CANT_ACCESS_PROCESS "Specified process could not be accessed"
#define E_CANT_ALLOCATE_MEMORY "Memory could not be allocated inside the chosen process"
#define E_CANT_WRITE_TO_PROCESS_MEMORY "Data could not be written to the address space of specified process"
#define E_CANT_CREATE_REMOTE_THREAD "Remote thread could not be created"

#define KERNEL_32_DLL_FILENAME "kernel32.dll"
#define FUNCNAME_LOAD_LIBRARY_A "LoadLibraryA"

VOID Notify(PCSTR message);
DWORD GetPidByName(PCSTR pszName);

INT main(INT argc, PCHAR argv[])
{
    if (argc < MIN_VALID_ARGC) {
        Notify(TEXTMSG_USAGE);
        return 1;
    }

    DWORD dwPid = GetPidByName(argv[2]);
    if (!dwPid) {
        Notify(E_PROCESS_NOT_FOUND);
        return 1;
    }
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (!hProc) {
        Notify(E_CANT_ACCESS_PROCESS);
        return 1;
    }

    LPVOID pAddr = (LPVOID)GetProcAddress(GetModuleHandle(KERNEL_32_DLL_FILENAME), FUNCNAME_LOAD_LIBRARY_A);
    if (!pAddr) {
        Notify(E_LOAD_LIBRARY_FUNCTION_NOT_FOUND);
        return 1;
    }

    CHAR szDllFullPath[MAX_PATH] = { 0 };
    GetFullPathNameA(argv[1], MAX_PATH, szDllFullPath, NULL);
    if (!sizeof(szDllFullPath)) {
        Notify(E_LIBRARY_NOT_FOUND);
        return 1;
    }
    LPVOID pArg = (LPVOID)VirtualAllocEx(hProc, NULL, strlen(szDllFullPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!pArg) {
        Notify(E_CANT_ALLOCATE_MEMORY);
        return 1;
    }
    if (!WriteProcessMemory(hProc, pArg, szDllFullPath, strlen(szDllFullPath), NULL)) {
        Notify(E_CANT_WRITE_TO_PROCESS_MEMORY);
        return 1;
    }

    if (!CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)pAddr, pArg, NULL, NULL)) {
        Notify(E_CANT_CREATE_REMOTE_THREAD);
        return 1;
    }

    CloseHandle(hProc);
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