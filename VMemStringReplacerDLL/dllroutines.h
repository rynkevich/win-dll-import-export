#pragma once

#define DLLEXPORT __declspec(dllexport)

INT DLLEXPORT WINAPI ReplaceString(PCSTR pszProcessName, PCSTR pszTarget, PCSTR pszReplacement);
INT DLLEXPORT WINAPI DumpMem(PCSTR pszProcessName, PCSTR pszDumpFilePath);