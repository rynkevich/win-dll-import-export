#pragma once

#include <vector>
#include <windows.h>

#define DLLEXPORT __declspec(dllexport)

void DLLEXPORT __stdcall ReplaceString(char *pszTarget, char *pszReplacement);
