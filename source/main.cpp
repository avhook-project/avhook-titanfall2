//
// Created by Vlad on 18.01.2024.
//
#include <Windows.h>
#include <thread>
#include "hooks/hooks.h"


namespace avh
{
    void MainThread(HMODULE hModule)
    {
        {
            auto hooks = hooks::HooksManager();

            while (!GetAsyncKeyState(VK_END))
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        FreeLibrary(hModule);
    }
}

[[maybe_unused]]
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        std::thread(avh::MainThread, hModule).detach();
    }

    return true;
}