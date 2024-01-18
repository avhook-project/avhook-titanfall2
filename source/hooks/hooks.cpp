//
// Created by Vlad on 18.01.2024.
//

#include "hooks.h"
#include "MinHook.h"
#include <d3d11.h>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_freetype.h>
#include <imgui/imgui_impl_win32.h>
#include <titanfall2_sdk/EntityList.h>
#include <titanfall2_sdk/ViewProjectionMatrix.h>
#include <optional>
#include <format>


void* originalSwapChainPresentFunction = nullptr;

HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
bool init = false;

void InitImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);
}

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
    using functype = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT);

    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);

            D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
            rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

            pDevice->CreateRenderTargetView(pBackBuffer, &rtv_desc, &mainRenderTargetView);
            pBackBuffer->Release();
            InitImGui();
            init = true;
        }

        else
            return reinterpret_cast<functype>(originalSwapChainPresentFunction)(pSwapChain, SyncInterval, Flags);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    for (size_t i = 0; i < 13; i++)
    {
        const auto ent = titanfall2_sdk::EntityList::Get().GetEntityById(i);

        if (!ent)
            continue;

        const auto pos = WorldToScreen(ent->GetOrigin());

        if (pos)
        {
            ImGui::GetForegroundDrawList()->AddLine({1920.f / 2, 1080}, *pos, ImColor(255, 0, 0));

        }
    }
    ImGui::Render();

    pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    using functype = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT);
    return reinterpret_cast<functype>(originalSwapChainPresentFunction)(pSwapChain, SyncInterval, Flags);
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