//
// Created by Vlad on 18.01.2024.
//

#include "hooks.h"
#include "MinHook.h"
#include <d3d11.h>
#include <functional>

void* originalSwapChainPresentFunction = nullptr;

HRESULT __fastcall hPresent(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags)
{


    using functype = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT);
    return reinterpret_cast<functype>(originalSwapChainPresentFunction)(pChain, SyncInterval, Flags);
}


namespace avh::hooks
{
    HooksManager &avh::hooks::HooksManager::Innit()
    {
        static auto obj = HooksManager();

        return obj;
    }

    HooksManager::HooksManager()
    {
        MH_Initialize();
        const auto swapChainPresentFunction = reinterpret_cast<uintptr_t>(GetModuleHandleA("dxgi.dll")) + 0x347c0;

        MH_CreateHook((void*)swapChainPresentFunction, (void*)hPresent, &originalSwapChainPresentFunction);

        MH_EnableHook(MH_ALL_HOOKS);
    }

    HooksManager::~HooksManager()
    {
        MH_RemoveHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }
}