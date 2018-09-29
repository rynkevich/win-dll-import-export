#pragma once

#define DLLIMPORT __declspec(dllimport)

INT DLLIMPORT WINAPI ReplaceString(DWORD dwPid, PCSTR pszTarget, PCSTR pszReplacement);
INT DLLIMPORT WINAPI DumpMem(DWORD dwPid, PCSTR pszDumpFilePath);