#include <stdio.h>
#include <windows.h>
#include "main.h"

#define MIN_VALID_ARGC 4
#define TEXTMSG_USAGE "Usage: <process_name> <string_to_replace> <new_string> "\
                      "[<dump_before_replace_filename>] [<dump_after_replace_filename>]"
#define TEXTMSG_PROCESS_NOT_FOUND "Process not found"
#define TEXTMSG_DONE "Done."

INT DumpMemAndReport(PCSTR pszProcessName, PCSTR pszDumpFilePath);

INT main(INT argc, PCHAR argv[])
{
    if (argc < MIN_VALID_ARGC) {
        puts(TEXTMSG_USAGE);
        return 1;
    }

    if (argc > MIN_VALID_ARGC && DumpMemAndReport(argv[1], argv[4])) {
        return 1;
    }
    if (ReplaceString(argv[1], argv[2], argv[3])) {
        puts(TEXTMSG_PROCESS_NOT_FOUND);
        getchar();
        return 1;
    } 
    if (argc > (MIN_VALID_ARGC + 1) && DumpMemAndReport(argv[1], argv[5])) {
        return 1;
    }

    puts(TEXTMSG_DONE);
    getchar();
    return 0;
}

INT DumpMemAndReport(PCSTR pszProcessName, PCSTR pszDumpFilePath) 
{
    CHAR szDumpFileFullPath[MAX_PATH];
    GetFullPathNameA(pszDumpFilePath, MAX_PATH, szDumpFileFullPath, NULL);
    if (DumpMem(pszProcessName, szDumpFileFullPath)) {
        puts(TEXTMSG_PROCESS_NOT_FOUND);
        getchar();
        return 1;
    }
    return 0;
}