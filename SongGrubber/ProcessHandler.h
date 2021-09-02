#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>

#include "Vector.h"

using namespace std;

#define MAX_PROCESSES 4096

typedef void (*callbackFunc)(WCHAR* exeName);

struct CachedProcName {
	WCHAR* procName;
	DWORD procPID;
};

struct CallbackInfo {
	DWORD* procList;
	callbackFunc returnFunc;
};

namespace ProcessHandler {
	void GetProcessNames(DWORD* processes, callbackFunc funcPtr);
}