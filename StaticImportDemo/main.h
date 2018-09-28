#pragma once

#define DLLIMPORT __declspec(dllimport)

INT DLLIMPORT WINAPI ReplaceString(PCSTR pszProcessName, PCSTR pszTarget, PCSTR pszReplacement);
INT DLLIMPORT WINAPI DumpMem(PCSTR pszProcessName, PCSTR pszDumpFilePath);