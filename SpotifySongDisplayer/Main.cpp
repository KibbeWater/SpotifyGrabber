#include "Main.h"

HINSTANCE grubber = 0;

DWORD lastRun = 0;
DWORD lastCacheUpdate = 0;

SpotifyStatus curStatus = SpotifyStatus::Not_Running;
SpotifyStatus oldStatus = SpotifyStatus::Not_Running;

//wchar_t* lastPlaying = (wchar_t*)L"";

wchar_t lastPlaying[256] = {};

bool newPlaying = false;

std::string premiumText("Spotify Premium");
std::wstring premium(premiumText.begin(), premiumText.end());

std::string freeText("Spotify Free");
std::wstring freeT(freeText.begin(), freeText.end());

std::string imeText("Default IME");
std::wstring IME(imeText.begin(), imeText.end());

std::string msctfimeText("MSCTFIME UI");
std::wstring msctfime(msctfimeText.begin(), msctfimeText.end());

std::string gdiText("GDI+ Window (Spotify.exe)");
std::wstring gdi(gdiText.begin(), gdiText.end());

std::string emptyText("");
std::wstring emptyT(emptyText.begin(), emptyText.end());

void callback(WCHAR* exeName) { //This callback is called for each window name it finds for each process
    std::wstring name(exeName);

    if (name.compare(emptyT) != 0)
        if (name.compare(gdi) != 0 && name.compare(msctfime) != 0 && name.compare(IME) != 0)
            if (name.compare(premium) == 0 || name.compare(freeT) == 0)
                curStatus = SpotifyStatus::Paused;
            else {
                curStatus = SpotifyStatus::Playing;
                if (wcscmp(lastPlaying, exeName) != 0)
                    newPlaying = true;
                //void* currentPtr = (void*)lastPlaying;

                wcscpy(lastPlaying, exeName);

                //lastPlaying = exeName;
                //free(currentPtr);
            }
    free(exeName);
}

int exitError(const char* errorMsg) {
    cout << errorMsg << endl;
    system("pause");
    return EXIT_FAILURE;
}

int main() {
    grubber = LoadLibrary(L"SongGrubber.dll");
    if (grubber == NULL)
        return exitError("Unable to load SongGrubber.dll");

    f_updatecache updateCache = (f_updatecache)GetProcAddress(grubber, "UpdateCache");
    if (updateCache == NULL)
        return exitError("Unable to load UpdateCache()");

    f_getspotifyprocesses getSpotify = (f_getspotifyprocesses)GetProcAddress(grubber, "GetSpotifyProcesses");
    if (getSpotify == NULL)
        return exitError("Unable to load GetSpotifyProcesses()");

    f_getprocessnames getProcessNames = (f_getprocessnames)GetProcAddress(grubber, "GetProcessNames");
    if (getProcessNames == NULL)
        return exitError("Unable to load GetProcessNames()");

    int cacheStatus = updateCache(); //Update cache so we can use if for the while loop
    if (cacheStatus == CACHE_SNAPSHOTERR)
        return exitError("Unable to open a snapshot");
    else if (cacheStatus == CACHE_NOTRUNNING)
        curStatus = SpotifyStatus::Not_Running;

    while (true) {
        DWORD tick = GetTickCount(); //wauw

        if (lastCacheUpdate + CACHE_UPDATE_TIMEOUT < tick) { //Update spotify cache
            int cacheStatus = updateCache();
            if (cacheStatus == CACHE_SNAPSHOTERR)
                return exitError("Unable to open a snapshot");
            else if (cacheStatus == CACHE_NOTRUNNING)
                curStatus = SpotifyStatus::Not_Running;
            lastCacheUpdate = tick;
        }

        if (lastRun + PROCESS_UPDATE_TIMEOUT < tick) { //Update spotify processes
            DWORD* procs = getSpotify(false);

            int spotifyProcessCount = 0;
            for (size_t i = 0; i < MAX_PROCESSES_SIZE; i++) {
                if (procs[i] == 0)
                    break;
                spotifyProcessCount++;
            }

            getProcessNames(procs, callback);

            lastRun = tick;
        }

        if (curStatus != oldStatus || newPlaying) {
            oldStatus = curStatus;
            newPlaying = false;
            switch (curStatus)
            {
                case SpotifyStatus::Not_Running:
                    cout << "Spotify is currently not running" << endl;
                    break;
                case SpotifyStatus::Paused:
                    cout << "Spotify is paused" << endl;
                    break;
                case SpotifyStatus::Playing:
                    if (lastPlaying != 0)
                        wcout << "Currently playing: " << lastPlaying << endl;
                    break;
            }
        }

        Sleep(1);
    }
}
