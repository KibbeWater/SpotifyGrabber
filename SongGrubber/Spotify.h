#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <comdef.h>

#include "ProcessHandler.h"

using namespace std;

#define DLLExport   __declspec( dllexport )
#define DLLImport   __declspec( dllimport )

typedef void (*callbackFunc)(WCHAR* exeName);

#define MAX_PROCESSES_SIZE MAX_PROCESSES * sizeof(DWORD)

extern "C" {
	DLLExport int UpdateCache();
	DLLExport DWORD* GetSpotifyProcesses(bool resetCache);
	DLLExport void GetProcessNames(DWORD* processes, callbackFunc callback);
}