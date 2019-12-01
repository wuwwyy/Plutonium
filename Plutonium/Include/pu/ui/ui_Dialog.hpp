
/*

    Plutonium library

    @file ui_Dialog.hpp
    @brief A Dialog is an easy way to ask the user to choose between several options.
    @author XorTroll

    @copyright Plutonium project - an easy-to-use UI framework for Nintendo Switch homebrew

*/

#pragma once
#include <pu/pu_String.hpp>
#include <pu/ui/render/render_Renderer.hpp>
#include <vector>

namespace pu::ui
{
    class Dialog
    {
        public:
            Dialog(String Title, String Content, u32 maxWidth = 1280);
            PU_SMART_CTOR(Dialog)
            ~Dialog();

            void AddOption(String Name);
            void SetCancelOption(String Name = "Cancel");
            void RemoveCancelOption();
            bool HasCancelOption();
            void SetIcon(std::string Icon);
            bool Hasicon();
            s32 Show(render::Renderer::Ref &Drawer, void *App);
            bool UserCancelled();
            bool IsOk();
        private:
            bool hcancel;
            String scancel;
            render::NativeFont titleFont;
            render::NativeFont contentFont;
            render::NativeFont optionFont;
            String title;
            String content;
            render::NativeTexture titleTexture;
            render::NativeTexture contentTexture;
            std::vector<String> options;
            std::vector<render::NativeTexture> optionTextures;
            s32 selectedOption;
            bool cancel;
            bool hasIcon;
            render::NativeTexture iconTexture;
            s32 prevosel;
            s32 pselfact;
            s32 selfact;
    };
}