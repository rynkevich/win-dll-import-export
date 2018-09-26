#pragma once

#define DLLEXPORT __declspec(dllexport)

void DLLEXPORT __stdcall ReplaceString(const char *pszTarget, const char *pszReplacement);
void DLLEXPORT __stdcall DumpMem(const char *pszDumpFilePath);