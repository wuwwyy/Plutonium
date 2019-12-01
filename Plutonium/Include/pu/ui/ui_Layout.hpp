
/*

    Plutonium library

    @file ui_Layout.hpp
    @brief Contains pu::Layout class, the object used to render within applications
    @author XorTroll

    @copyright Plutonium project - an easy-to-use UI framework for Nintendo Switch homebrew

*/

#pragma once
#include <pu/ui/ui_Container.hpp>
#include <functional>

namespace pu::ui
{
    class Layout : public Container
    {
        public:
            Layout();
            PU_SMART_CTOR(Layout)
            ~Layout();

            bool HasChilds();
            virtual void OnInput(u64 Down, u64 Up, u64 Held, Touch Pos) = 0;
            virtual bool OnClose() = 0;
            virtual void OnTick() = 0;
            void SetBackgroundImage(std::string Path);
            void SetBackgroundColor(Color Color);
            void SimulateTouch(Touch Custom);
            Touch GetSimulatedTouch();
            render::NativeTexture GetBackgroundImageTexture();
            Color GetBackgroundColor();
            bool HasBackgroundImage();
        private:
            bool hasimage;
            Color overbgcolor;
            Touch simtouch;
            render::NativeTexture overbgtex;
    };
}