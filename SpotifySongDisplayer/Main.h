#pragma once
#include <Windows.h>
#include <iostream>
#include <libloaderapi.h>

#include <vector>

using namespace std;

typedef void (*callbackFunc)(WCHAR* exeName);

typedef int (*f_updatecache)();
typedef DWORD* (*f_getspotifyprocesses)(bool resetCache);
typedef void (*f_getprocessnames)(DWORD* processes, callbackFunc callback);

#define CACHE_SUCCESS 0
#define CACHE_SNAPSHOTERR 1
#define CACHE_NOTRUNNING 2

#define MAX_PROCESSES 4096
#define MAX_PROCESSES_SIZE MAX_PROCESSES * sizeof(DWORD)

#define PROCESS_UPDATE_TIMEOUT 750
#define CACHE_UPDATE_TIMEOUT 5000

enum class SpotifyStatus {
	Not_Running,
	Paused,
	Playing
};