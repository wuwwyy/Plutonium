
/*

    Plutonium library

    @file ui_Application.hpp
    @brief An Application is the base to use the UI system of this library.
    @author XorTroll

    @copyright Plutonium project - an easy-to-use UI framework for Nintendo Switch homebrew

*/

#pragma once
#include <vector>
#include <functional>
#include <chrono>
#include <stack>
#include <pu/ui/ui_Dialog.hpp>
#include <pu/ui/ui_Layout.hpp>
#include <pu/ui/ui_Overlay.hpp>

namespace pu::ui
{
    class Application
    {
        public:
            Application(render::Renderer::Ref Renderer);
            PU_SMART_CTOR(Application)

            void LoadLayout(Layout::Ref layout);
            void PopLayout();

            void Prepare();
            // Force create a derived Application which should initialize everything here
            virtual void OnLoad() = 0;

            s32 ShowDialog(Dialog::Ref &ToShow);
            int CreateShowDialog(String Title, String Content, std::vector<String> Options, bool UseLastOptionAsCancel, std::string Icon = "");

            void StartOverlay(std::shared_ptr<ui::Overlay> overlay);
            void StartOverlayWithTimeout(std::shared_ptr<ui::Overlay> overlay, u64 Milli);

            void EndOverlay();
            void Show();
            void ShowWithFadeIn();
            bool IsShown();
            bool CallForRender();
            bool CallForRenderWithRenderOver(std::function<bool(render::Renderer::Ref&)> RenderFunc);
            void FadeIn();
            void FadeOut();
            bool IsFadedIn();
            void SetFadeAlphaAmountPerFrame(u8 Alpha);
            void OnRender();
            void Close();
            void CloseWithFadeOut();
        protected:
            bool loaded;
            bool rover;
            std::function<bool(render::Renderer::Ref&)> rof;
            bool show;
            u8 aapf;
            s32 fadea;
            bool closefact;
            std::stack<Layout::Ref> layoutStack;
            u64 tmillis;
            std::chrono::steady_clock::time_point tclock;
            bool fovl;
            bool ffovl;
            Overlay::Ref ovl;
            render::Renderer::Ref rend;
    };
}