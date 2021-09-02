#include <Windows.h>
#include <iostream>

#include "Spotify.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            std::cout << "[Spotify Grabber] Initialized!" << std::endl;
            break;
    }
    return TRUE;
}

