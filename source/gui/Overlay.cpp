//
// Created by Vlad on 20.01.2024.
//

#include "Overlay.h"
#include <stdexcept>
#include <d3d11.h>

#include <imgui/imgui.h>

#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_freetype.h>

#include "windows/BaseWindow.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace avh::gui
{

    LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static auto& overlay = Overlay::Get();

        if (uMsg == WM_KEYDOWN && wParam == VK_INSERT && !(lParam & (1 << 30)))
            overlay.ToggleShow();

        if (!overlay.IsShouldShow())
            return CallWindowProc(overlay.m_originalWndProcHandler, hWnd, uMsg, wParam, lParam);

        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return TRUE;
    }

    Overlay::Overlay(IDXGISwapChain *swapChain)
    {
        if (swapChain == nullptr)
            throw std::runtime_error("Overlay construction failed: 'swapChain' cannot be nullptr. Ensure that the"
                                     "swapChain is initialized correctly before constructing the Overlay.");

        m_device = GetD3D11DeviceFromSwapChain(swapChain);

        if (m_device == nullptr)
            throw std::runtime_error("Overlay construction failed: 'device' cannot be nullptr. Ensure that the"
                                     "D3D11Device is initialized correctly before constructing the Overlay.");

        m_device->GetImmediateContext(&m_context);

        DXGI_SWAP_CHAIN_DESC sd;
        swapChain->GetDesc(&sd);
        m_windowHandle = sd.OutputWindow;


        D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
        rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

        m_device->CreateRenderTargetView(GetBackBufferFromSwapChain(swapChain).get(),
                                         &rtv_desc,
                                         &m_overlayRenderTargetView);
        InitImgui();

        m_originalWndProcHandler = (WNDPROC)SetWindowLongPtr(m_windowHandle, GWLP_WNDPROC, (LONG_PTR)WndProc);
    }

    Overlay &Overlay::Get(IDXGISwapChain *swapChain)
    {
        static auto obj = Overlay(swapChain);

        return obj;
    }
    ID3D11Device *Overlay::GetD3D11DeviceFromSwapChain(IDXGISwapChain *swapChain)
    {
        ID3D11Device* device = nullptr;
        swapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device));

        return device;
    }

    std::unique_ptr<ID3D11Texture2D, void(*)(ID3D11Texture2D*)>
    Overlay::GetBackBufferFromSwapChain(IDXGISwapChain *swapChain)
    {
        ID3D11Texture2D* backBuffer;
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer));

        return {backBuffer, [](ID3D11Texture2D* res) -> void {if (res) res->Release();}};
    }

    Overlay::~Overlay()
    {
        SetWindowLongPtr(m_windowHandle, GWLP_WNDPROC, (LONG_PTR)m_originalWndProcHandler);
    }

    void Overlay::InitImgui()
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

        ImGui_ImplWin32_Init(m_windowHandle);
        ImGui_ImplDX11_Init(m_device, m_context);

        ImFontConfig cfg;
        cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;
        static ImWchar ranges[] = { 0x1, 0xFFFD, 0 };
        io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\verdanab.ttf)", 13.f, &cfg, ranges);

        window =  std::make_unique<window::BaseWindow>("AVhook");
        SetImGuiTheme();
    }

    void Overlay::Render()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        RenderEsp();

        if (IsShouldShow())
            RenderMenu();

        ImGui::Render();
        m_context->OMSetRenderTargets(1, &m_overlayRenderTargetView, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void Overlay::RenderMenu()
    {
        window->Render();

    }

    void Overlay::RenderEsp()
    {

    }

    void Overlay::SetImGuiTheme()
    {
        auto& style = ImGui::GetStyle();
        auto& theme = style.Colors;

        style.FrameBorderSize = 1;
        style.AntiAliasedLinesUseTex = false;
        style.AntiAliasedLines = false;
        style.AntiAliasedFill = true;
        style.ScrollbarRounding = 0.f;
        style.WindowMinSize = ImVec2(10, 10);
        style.WindowPadding= {0, 0};

        theme[ImGuiCol_Text] = ImColor(255, 255, 255);
        theme[ImGuiCol_TextDisabled] = ImColor(199, 199, 199);
        theme[ImGuiCol_WindowBg] = ImColor(19, 19, 19);
        theme[ImGuiCol_ChildBg] = ImColor();
        theme[ImGuiCol_PopupBg] = ImColor(19, 19, 19);
        theme[ImGuiCol_Border] = ImColor(255, 95, 95);
        theme[ImGuiCol_BorderShadow] = ImColor();
        theme[ImGuiCol_FrameBg] = ImColor();
        theme[ImGuiCol_FrameBgHovered] = ImColor(255, 102, 102, 90);
        theme[ImGuiCol_FrameBgActive] = ImColor(255, 255, 255, 90);
        theme[ImGuiCol_Header] = ImColor(255, 95, 95);
        theme[ImGuiCol_HeaderActive] = ImColor(255, 255, 255, 90);
        theme[ImGuiCol_HeaderHovered] = ImColor(255, 102, 102, 90);
        theme[ImGuiCol_CheckMark] = ImColor(255, 95, 95);
        theme[ImGuiCol_Button] = ImColor();
        theme[ImGuiCol_ButtonHovered] = ImColor(255, 102, 102, 90);
        theme[ImGuiCol_ButtonActive] = ImColor(255, 255, 255, 90);
        theme[ImGuiCol_TextSelectedBg] = ImColor(255, 95, 95);
        theme[ImGuiCol_SliderGrab] = ImColor(255, 95, 95);
        theme[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255, 90);
        theme[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 255, 255, 90);
    }

} // avh