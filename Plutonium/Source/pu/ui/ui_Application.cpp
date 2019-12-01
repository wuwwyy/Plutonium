#include <pu/ui/ui_Application.hpp>

namespace pu::ui
{
    Application::Application(render::Renderer::Ref Renderer)
    {
        this->rend = Renderer;
        this->rend->Initialize();
        this->show = false;
        this->rover = false;
        this->ovl = nullptr;
        this->closefact = false;
        this->fovl = false;
        this->ffovl = false;
        this->loaded = false;
        this->rof = [](render::Renderer::Ref&) -> bool { return true; };
        this->fadea = 255;
        this->aapf = 35;
    }

    void Application::LoadLayout(Layout::Ref layout)
    {
        this->layoutStack.emplace(layout);
    }

    void Application::PopLayout()
    {
        if (this->layoutStack.size() > 1)
        {
            if (this->layoutStack.top()->OnClose())
                this->layoutStack.pop();
        }
    }

    void Application::Prepare()
    {
        if(!this->loaded)
        {
            this->OnLoad();
            this->loaded = true;
        }
    }

    s32 Application::ShowDialog(Dialog::Ref &ToShow)
    {
        return ToShow->Show(this->rend, this);
    }

    int Application::CreateShowDialog(String Title, String Content, std::vector<String> Options, bool UseLastOptionAsCancel, std::string Icon)
    {
        Dialog dlg(Title, Content);
        for(s32 i = 0; i < Options.size(); i++)
        {
            if(UseLastOptionAsCancel && (i == Options.size() - 1)) dlg.SetCancelOption(Options[i]);
            else dlg.AddOption(Options[i]);
        }
        if(!Icon.empty()) dlg.SetIcon(Icon);
        int opt = dlg.Show(this->rend, this);
        if(dlg.UserCancelled()) opt = -1;
        else if(!dlg.IsOk()) opt = -2;
        return opt;
    }

    void Application::StartOverlay(std::shared_ptr<ui::Overlay> overlay)
    {
        if(this->ovl == nullptr) this->ovl = overlay;
    }

    void Application::StartOverlayWithTimeout(std::shared_ptr<ui::Overlay> overlay, u64 Milli)
    {
        if(this->ovl == nullptr)
        {
            this->ovl = overlay;
            this->tmillis = Milli;
            this->tclock = std::chrono::steady_clock::now();
        }
    }

    void Application::EndOverlay()
    {
        if(this->ovl != nullptr)
        {
            this->ovl->NotifyEnding(false);
            this->tmillis = 0;
            this->ovl = nullptr;
            this->fovl = false;
            this->ffovl = false;
        }
    }

    void Application::Show()
    {
        if(!this->loaded) return;
        if(this->layoutStack.size() == 0) return;
        this->show = true;
        while(this->show) this->CallForRender();
    }

    void Application::ShowWithFadeIn()
    {
        this->FadeIn();
        this->Show();
    }

    bool Application::IsShown()
    {
        return this->show;
    }

    bool Application::CallForRender()
    {
        if(!this->loaded) return false;
        if(this->layoutStack.size() == 0) return false;
        bool c = true;
        this->rend->InitializeRender(this->layoutStack.top()->GetBackgroundColor());
        this->OnRender();
        if(this->rover)
        {
            c = (this->rof)(this->rend);
            this->rover = false;
            this->rof = [](render::Renderer::Ref &Drawer) -> bool { return true; };
        }
        this->rend->FinalizeRender();
        return c;
    }

    bool Application::CallForRenderWithRenderOver(std::function<bool(render::Renderer::Ref &Drawer)> RenderFunc)
    {
        this->rover = true;
        this->rof = RenderFunc;
        return this->CallForRender();
    }

    void Application::FadeIn()
    {
        fadea = 0;
        while(true)
        {
            CallForRender();
            fadea += aapf;
            if(fadea > 255)
            {
                fadea = 255;
                CallForRender();
                break;
            }
        }
    }

    void Application::FadeOut()
    {
        fadea = 255;
        while(true)
        {
            CallForRender();
            fadea -= aapf;
            if(fadea < 0)
            {
                fadea = 0;
                CallForRender();
                break;
            }
        }
    }

    bool Application::IsFadedIn()
    {
        return (fadea > 0);
    }

    void Application::SetFadeAlphaAmountPerFrame(u8 Alpha)
    {
        aapf = Alpha;
    }

    void Application::OnRender()
    {
        hidScanInput();
        u64 d = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 u = hidKeysUp(CONTROLLER_P1_AUTO);
        u64 h = hidKeysHeld(CONTROLLER_P1_AUTO);
        u64 th = hidKeysDown(CONTROLLER_HANDHELD);
        Touch tch = Touch::Empty;
        if(th & KEY_TOUCH)
        {
            touchPosition nxtch;
            hidTouchRead(&nxtch, 0);
            tch.X = nxtch.px;
            tch.Y = nxtch.py;
        }
        auto simtch = this->layoutStack.top()->GetSimulatedTouch();
        if(!simtch.IsEmpty()) tch = simtch;
        this->layoutStack.top()->PreRender();
        this->layoutStack.top()->OnTick();
        if(this->layoutStack.top()->HasBackgroundImage()) this->rend->RenderTexture(this->layoutStack.top()->GetBackgroundImageTexture(), 0, 0);
        if(!this->rover) this->layoutStack.top()->OnInput(d, u, h, tch);
        if(this->layoutStack.top()->HasChilds()) for(s32 i = 0; i < this->layoutStack.top()->GetCount(); i++)
        {
            auto elm = this->layoutStack.top()->At(i);
            if(elm->IsVisible())
            {
                elm->OnRender(this->rend, elm->GetProcessedX(), elm->GetProcessedY());
                if(!this->rover) elm->OnInput(d, u, h, tch);
            }
        }
        if(this->ovl != NULL)
        {
            bool rok = this->ovl->Render(this->rend);
            if(this->tmillis > 0)
            {
                auto nclk = std::chrono::steady_clock::now();
                u64 cctime = std::chrono::duration_cast<std::chrono::milliseconds>(nclk - this->tclock).count();
                if(cctime >= this->tmillis) this->ovl->NotifyEnding(true);
            }
            if(!rok) this->EndOverlay();
        }
        this->rend->RenderRectangleFill({ 0, 0, 0, 255 - (u8)fadea }, 0, 0, 1280, 720);
    }

    void Application::Close()
    {
        this->show = false;
        this->rend->Finalize();
    }

    void Application::CloseWithFadeOut()
    {
        this->FadeOut();
        this->Close();
    }
}