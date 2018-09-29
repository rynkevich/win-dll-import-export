#include <stdio.h>
#include <windows.h>

#define MIN_VALID_ARGC 5
#define ARGC_WHEN_DUMP_BEFORE (MIN_VALID_ARGC + 1)
#define ARGC_WHEN_DUMP_AFTER (ARGC_WHEN_DUMP_BEFORE + 1)
#define TEXTMSG_USAGE "Usage: <dll_filename> <process_name> <string_to_replace> <new_string> "\
                      "[<dump_before_replace_filename>] [<dump_after_replace_filename>]"
#define TEXTMSG_PROCESS_NOT_FOUND "Process not found"
#define TEXTMSG_CANT_LOAD_LIBRARY "Can not load the dynamic library"
#define TEXTMSG_CANT_LOCATE_FUNCTION "Can not locate function"
#define TEXTMSG_DONE "Done."

#define FUNCNAME_DUMP_MEM "?DumpMem@@YGHPBD0@Z"
#define FUNCNAME_REPLACE_STRING "?ReplaceString@@YGHPBD00@Z"

typedef INT WINAPI TDumpMem(PCSTR, PCSTR);
typedef INT WINAPI TReplaceString(PCSTR, PCSTR, PCSTR);

INT DumpMemAndReport(TDumpMem fnDumpMem, PCSTR pszProcessName, PCSTR pszDumpFilePath);

INT main(INT argc, PCHAR argv[])
{
    if (argc < MIN_VALID_ARGC) {
        puts(TEXTMSG_USAGE);
        return 1;
    }

    HMODULE hVMemStringReplacerDll = LoadLibrary(argv[1]);
    if (!hVMemStringReplacerDll) {
        puts(TEXTMSG_CANT_LOAD_LIBRARY);
        getchar();
        return 1;
    }

    TDumpMem *fnDumpMem = (TDumpMem *)GetProcAddress(hVMemStringReplacerDll, FUNCNAME_DUMP_MEM);
    if (!fnDumpMem) {
        puts(TEXTMSG_CANT_LOCATE_FUNCTION);
        getchar();
        return 1;
    }
    TReplaceString *fnReplaceString = (TReplaceString *)GetProcAddress(hVMemStringReplacerDll, FUNCNAME_REPLACE_STRING);
    if (!fnReplaceString) {
        puts(TEXTMSG_CANT_LOCATE_FUNCTION);
        getchar();
        return 1;
    }

    if (argc >= ARGC_WHEN_DUMP_BEFORE && DumpMemAndReport(fnDumpMem, argv[2], argv[5])) {
        return 1;
    }
    if (fnReplaceString(argv[2], argv[3], argv[4])) {
        puts(TEXTMSG_PROCESS_NOT_FOUND);
        getchar();
        return 1;
    }
    if (argc >= ARGC_WHEN_DUMP_AFTER && DumpMemAndReport(fnDumpMem, argv[2], argv[6])) {
        return 1;
    }

    FreeLibrary(hVMemStringReplacerDll);

    puts(TEXTMSG_DONE);
    getchar();
    return 0;
}

INT DumpMemAndReport(TDumpMem fnDumpMem, PCSTR pszProcessName, PCSTR pszDumpFilePath)
{
    CHAR szDumpFileFullPath[MAX_PATH];
    GetFullPathNameA(pszDumpFilePath, MAX_PATH, szDumpFileFullPath, NULL);
    if (fnDumpMem(pszProcessName, szDumpFileFullPath)) {
        puts(TEXTMSG_PROCESS_NOT_FOUND);
        getchar();
        return 1;
    }
    return 0;
}