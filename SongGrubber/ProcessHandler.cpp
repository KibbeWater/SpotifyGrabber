#include "ProcessHandler.h"

BOOL CALLBACK ProcNameCallback(_In_ HWND hwnd, _In_ LPARAM lParam) {
	CallbackInfo inf = *((CallbackInfo*)lParam);

	DWORD winId;
	GetWindowThreadProcessId(hwnd, &winId);
	if (winId == 13080)
		Sleep(1);

	for (size_t i = 0; i < MAX_PROCESSES/4; i++) {
		DWORD PID = inf.procList[i];
		if (PID == 0)
			break;
		if (PID == winId) {
			LPWSTR winText = (WCHAR*)malloc(2048);
			if (winText == 0)
				continue;
			GetWindowTextW(hwnd, winText, 2048);
			if (winText != NULL) {
				inf.returnFunc(winText);
			}
		}
	}

	return true;
}

void ProcessHandler::GetProcessNames(DWORD* processes, callbackFunc funcPtr) {
	CallbackInfo inf{};
	inf.procList = processes;
	inf.returnFunc = funcPtr;
	EnumWindows(ProcNameCallback, (LPARAM)(&inf));
}