//
// Created by Vlad on 18.01.2024.
//

#include "hooks.h"
#include "MinHook.h"
#include <d3d11.h>
#include <imgui/imgui.h>
#include <titanfall2_sdk/EntityList.h>
#include <titanfall2_sdk/ViewProjectionMatrix.h>
#include <optional>
#include "gui/Overlay.h"


void* originalSwapChainPresentFunction = nullptr;


std::optional<ImVec2> WorldToScreen(const uml::Vector3& worldPos)
{
    auto projected = (titanfall2_sdk::GetViewProjectionMatrix() * worldPos).transpose();

    if (projected.at(0,3) <= 0.f)
        return std::nullopt;

    projected /= projected.at(0, 3);

    const auto [width, height] = ImGui::GetMainViewport()->Size;

    const auto out = projected * uml::matrix::to_screen_matrix(width, height);

    return ImVec2(out.at(0,0), out.at(0, 1));
}

HRESULT __fastcall hPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    auto& overlay = avh::gui::Overlay::Get(pSwapChain);
    overlay.Render();


    using functype = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT);
    return reinterpret_cast<functype>(originalSwapChainPresentFunction)(pSwapChain, SyncInterval, Flags);
}


namespace avh::hooks
{
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