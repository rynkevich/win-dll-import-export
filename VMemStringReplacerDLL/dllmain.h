#pragma once

#define DLLEXPORT __declspec(dllexport)

INT DLLEXPORT WINAPI ReplaceString(DWORD dwPid, PCSTR pszTarget, PCSTR pszReplacement);
INT DLLEXPORT WINAPI DumpMem(DWORD dwPid, PCSTR pszDumpFilePath);