#include "Spotify.h"

ImVector<DWORD> g_vSpotifyList;

extern "C" {
	DLLExport int UpdateCache() { // 0 - All good, 1 - Snapshot error, 2 - Spotify not found
		int Status = 2;

		g_vSpotifyList.clear();

		PROCESSENTRY32 Entry;
		Entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return 1;

		if (Process32First(hSnapshot, &Entry))
			while (Process32Next(hSnapshot, &Entry))
				if (wcscmp(Entry.szExeFile, L"Spotify.exe") == 0) {
					Status = 0;
					g_vSpotifyList.push_back(Entry.th32ProcessID);
				}

		CloseHandle(hSnapshot);
		return Status;
	}

	DLLExport DWORD* GetSpotifyProcesses(bool resetCache) {
		if (resetCache)
			UpdateCache();
		DWORD* spotifyProcs = (DWORD*)VirtualAlloc(NULL, MAX_PROCESSES_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (spotifyProcs == NULL)
			return NULL;
		for (size_t i = 0; i < g_vSpotifyList.Size; i++)
			spotifyProcs[i] = g_vSpotifyList[i];
		return spotifyProcs;
	}

	DLLExport void GetProcessNames(DWORD* processes, callbackFunc callback) {
		ProcessHandler::GetProcessNames(processes, callback);
	}
}