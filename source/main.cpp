//
// Created by Vlad on 18.01.2024.
//
#include <Windows.h>
#include <thread>

void HackThread(HMODULE hModule)
{
    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(10);
    }

    FreeLibrary(hModule);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        std::thread(HackThread, hModule).detach();
    }
    return TRUE;
}