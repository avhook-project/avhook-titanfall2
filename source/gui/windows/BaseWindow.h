//
// Created by Vlad on 20.01.2024.
//

#pragma once
#include <string>
#include <memory>
#include <d3d11.h>


class ID3D11Device;
class ID3D11ShaderResourceView;
struct ImFont;


namespace avh::gui::window
{
    class BaseWindow
    {
    public:
        explicit BaseWindow(const std::string& tittle, std::unique_ptr<ID3D11ShaderResourceView> icon= nullptr);
        virtual void Render();
    protected:
        void DrawWindowHeader();
        void Body();
        uint32_t m_imguiWindowStyle;

        std::unique_ptr<ImFont> m_pFontSmall;
        std::unique_ptr<ImFont> m_pFontSmallBold;
        std::unique_ptr<ImFont> m_pFontMedium;
        std::unique_ptr<ImFont> m_pFontMediumBold;
        std::unique_ptr<ImFont> m_pFontLargeBold;
    private:
        std::unique_ptr<ID3D11ShaderResourceView> m_windowIcon;
        std::string m_tittle;
    };
} // avh
