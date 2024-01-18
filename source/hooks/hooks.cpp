//
// Created by Vlad on 18.01.2024.
//

#include "hooks.h"
#include "MinHook.h"


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
    }

    HooksManager::~HooksManager()
    {
        MH_RemoveHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }
}