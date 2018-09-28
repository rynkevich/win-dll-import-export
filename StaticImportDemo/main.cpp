#include <stdio.h>
#include <windows.h>
#include "main.h"

#define MIN_VALID_ARGC 4
#define USAGE_MSG "Usage: <process_name> <string_to_replace> <new_string> [<dump_before_replace_filename>] [<dump_after_replace_filename>]"

INT main(INT argc, PCHAR argv[])
{
    if (argc < MIN_VALID_ARGC) {
        puts(USAGE_MSG);
        return 1;
    }

    CHAR szDumpBeforeReplaceFullPath[MAX_PATH];
    CHAR szDumpAfterReplaceFullPath[MAX_PATH];
    GetFullPathNameA(argv[4], MAX_PATH, szDumpBeforeReplaceFullPath, NULL);
    GetFullPathNameA(argv[5], MAX_PATH, szDumpAfterReplaceFullPath, NULL);

    if (argc > MIN_VALID_ARGC + 1) {
        DumpMem(argv[1], argv[4]);
    }

    INT iErrCode;
    if (iErrCode = ReplaceString(argv[1], argv[2], argv[3])) {
        puts("Process not found.");
    } else {
        puts("Done.");
        if (argc > MIN_VALID_ARGC + 2) {
            DumpMem(argv[1], argv[5]);
        }
    }

    getchar();
    return 0;
}