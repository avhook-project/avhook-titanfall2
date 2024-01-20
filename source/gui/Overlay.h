//
// Created by Vlad on 20.01.2024.
//

#pragma once
#include <memory>
#include <Windows.h>
#include <functional>
#include "windows/BaseWindow.h"

class IDXGISwapChain;
class ID3D11Device;
class ID3D11Texture2D;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;


namespace avh::gui
{
    class Overlay
    {
        friend LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:

        static Overlay &Get(IDXGISwapChain *swapChain = nullptr);

        Overlay(const Overlay &) = delete;

        Overlay &operator=(const Overlay &) = delete;

        void Render();

        [[nodiscard]] bool IsShouldShow() const
        { return m_show; };

        void ToggleShow()
        { m_show = !m_show; };

    private:
        bool m_show = false;
        ID3D11DeviceContext *m_context = nullptr;
        ID3D11Device *m_device = nullptr;
        WNDPROC m_originalWndProcHandler = nullptr;
        HWND m_windowHandle = nullptr;
        ID3D11RenderTargetView *m_overlayRenderTargetView = nullptr;
        std::unique_ptr<window::BaseWindow> window;

        void RenderMenu();

        void RenderEsp();

        explicit Overlay(IDXGISwapChain *swapChain);

        [[nodiscard]]
        static ID3D11Device *GetD3D11DeviceFromSwapChain(IDXGISwapChain *swapChain);

        [[nodiscard]]
        static std::unique_ptr<ID3D11Texture2D, void (*)(ID3D11Texture2D *)>
        GetBackBufferFromSwapChain(IDXGISwapChain *swapChain);

        ~Overlay();

        void InitImgui();
        void SetImGuiTheme();
    };
} // avh