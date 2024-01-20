//
// Created by Vlad on 20.01.2024.
//

#include "BaseWindow.h"
#include <imgui/imgui.h>
#include <imgui/imgui_freetype.h>


namespace avh::gui::window
{

    void BaseWindow::DrawWindowHeader()
    {

        ImGui::GetWindowDrawList()-> AddLine(ImGui::GetWindowPos()+ImVec2{0,15},
                                             ImGui::GetWindowPos()+ImVec2(ImGui::GetWindowWidth(),15),
                                             (ImColor)ImGui::GetStyle().Colors[ImGuiCol_Border]);

        if (m_windowIcon){
            ImGui::Image(m_windowIcon.get(), ImVec2(16, 16));
            ImGui::SameLine();
        }
        ImGui::PushFont(m_pFontSmallBold.get());
        ImGui::SetCursorPos({4,2});
        ImGui::Text(m_tittle.c_str());
        ImGui::PopFont();
        const ImVec2 windowSize = ImGui::GetWindowSize();

        // Setting position of button
        ImGui::SetCursorPos(ImVec2(windowSize.x - 16, 0));
        auto oldPos = ImGui::GetCursorPos();

        ImGui::PushFont(m_pFontMediumBold.get());
        {
            ImGui::Button("###close", ImVec2(16, 16));
            ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + oldPos + ImVec2(4, 0),
                                                ImColor(255, 255, 255),
                                                "×");
        }
        ImGui::PopFont();
        ImGui::SetCursorPos({0,15});
    }

    BaseWindow::BaseWindow(const std::string &tittle, std::unique_ptr<ID3D11ShaderResourceView> icon)
    {
        m_tittle = tittle;
        //m_windowIcon = std::move(icon);
        m_imguiWindowStyle =
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar|
                ImGuiWindowFlags_NoScrollWithMouse;

        ImFontConfig fontBuilderConfig;
        fontBuilderConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;
        const auto imGuiIo = ImGui::GetIO();

        static ImWchar ranges[] = { 0x1, 0xFFFF, 0 };

        m_pFontSmall = std::unique_ptr<ImFont>(imGuiIo.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\verdana.ttf)",
                                                                                 12.f, &fontBuilderConfig, ranges));

        m_pFontSmallBold = std::unique_ptr<ImFont>(imGuiIo.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\verdanab.ttf)",
                                                                                     12.f, &fontBuilderConfig, ranges));

        m_pFontMedium = std::unique_ptr<ImFont>(imGuiIo.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\verdana.ttf)",
                                                                                  15.f, &fontBuilderConfig, ranges));

        m_pFontMediumBold = std::unique_ptr<ImFont>(imGuiIo.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\verdanab.ttf)",
                                                                                      15.f, &fontBuilderConfig, ranges));

        m_pFontLargeBold = std::unique_ptr<ImFont>(imGuiIo.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\verdanab.ttf)",
                                                                                      25.f, &fontBuilderConfig, ranges));

    }

    void BaseWindow::Render()
    {
        ImGui::Begin(m_tittle.c_str(), nullptr, m_imguiWindowStyle);
        {
            DrawWindowHeader();
            Body();
            ImGui::End();
        }
    }

    void BaseWindow::Body()
    {
        ImGui::SetWindowSize({496,496});
        ImGui::GetStyle().ItemSpacing = {-1,0};

        ImGui::PushFont(m_pFontSmallBold.get());

        ImGui::PushItemWidth(10);
        ImGui::Button("General",{0,16});
        ImGui::SameLine();
        ImGui::Button("Aim Bot",{0,16});
        ImGui::SameLine();
        ImGui::Button("Visuals",{0,16});

        ImGui::PopFont();

        ImGui::GetWindowDrawList()-> AddLine(ImGui::GetWindowPos()+ImVec2{0,30},
                                             ImGui::GetWindowPos()+ImVec2(ImGui::GetWindowWidth(),30),
                                             (ImColor)ImGui::GetStyle().Colors[ImGuiCol_Border]);

        ImGui::ShowDemoWindow();
    }
} // avh