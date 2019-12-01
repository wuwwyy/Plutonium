#include <pu/ui/ui_Dialog.hpp>
#include <pu/ui/ui_Application.hpp>
#include <cmath>

namespace pu::ui
{
    Dialog::Dialog(String Title, String Content, u32 maxWidth)
    {
        this->titleFont = render::LoadDefaultFont(30);
        this->contentFont = render::LoadDefaultFont(20);
        this->optionFont = render::LoadDefaultFont(18);
        this->title = Title;
        this->content = Content;
        this->titleTexture = render::RenderText(this->titleFont, Title, { 10, 10, 10, 255 });
        this->contentTexture = render::RenderText(this->contentFont, Content, { 20, 20, 20, 255 }, maxWidth - 90, 4);
        this->selectedOption = 0;
        this->prevosel = 0;
        this->selfact = 255;
        this->pselfact = 0;
        this->hasIcon = false;
        this->cancel = false;
        this->hcancel = false;
    }

    Dialog::~Dialog()
    {
        if(this->titleTexture != NULL)
        {
            render::DeleteTexture(this->titleTexture);
            this->titleTexture = NULL;
        }
        if(this->contentTexture != NULL)
        {
            render::DeleteTexture(this->contentTexture);
            this->contentTexture = NULL;
        }
        if(this->hasIcon && (this->iconTexture != NULL))
        {
            render::DeleteTexture(this->iconTexture);
            this->iconTexture = NULL;
            this->hasIcon = false;
        }
        for(auto &opt: this->optionTextures) render::DeleteTexture(opt);
    }

    void Dialog::AddOption(String Name)
    {
        this->optionTextures.push_back(render::RenderText(this->optionFont, Name, { 10, 10, 10, 255 }));
        this->options.push_back(Name);
    }

    void Dialog::SetCancelOption(String Name)
    {
        this->hcancel = true;
        this->scancel = Name;
    }

    void Dialog::RemoveCancelOption()
    {
        this->hcancel = false;
        this->scancel = "";
    }

    bool Dialog::HasCancelOption()
    {
        return this->hcancel;
    }

    void Dialog::SetIcon(std::string Icon)
    {
        if(this->hasIcon) render::DeleteTexture(this->iconTexture);
        this->iconTexture = render::LoadImage(Icon);
        this->hasIcon = true;
    }

    bool Dialog::Hasicon()
    {
        return this->hasIcon;
    }

    s32 Dialog::Show(render::Renderer::Ref &Drawer, void *App)
    {
        if(this->hcancel) this->AddOption(this->scancel);
        if(this->optionTextures.empty()) return 0;
        s32 dw = (20 * (this->optionTextures.size() - 1)) + 250;
        for(s32 i = 0; i < this->optionTextures.size(); i++)
        {
            s32 tw = render::GetTextureWidth(optionTextures[i]);
            dw += tw + 20;
        }
        if(dw > 1280) dw = 1280;
        s32 icm = 30;
        s32 elemh = 60;
        s32 tdw = render::GetTextureWidth(this->contentTexture) + 90;
        if(tdw > dw) dw = tdw;
        tdw = render::GetTextureWidth(this->titleTexture) + 90;
        if(tdw > dw) dw = tdw;
        s32 ely = render::GetTextureHeight(this->titleTexture) + render::GetTextureHeight(this->contentTexture) + 140;
        if(this->hasIcon)
        {
            s32 tely = render::GetTextureHeight(this->iconTexture) + icm + 25;
            if(tely > ely) ely = tely;
            tdw = render::GetTextureWidth(this->contentTexture) + 90 + render::GetTextureWidth(this->iconTexture) + 20;
            if(tdw > dw) dw = tdw;
            tdw = render::GetTextureWidth(this->titleTexture) + 90 + render::GetTextureWidth(this->iconTexture) + 20;
            if(tdw > dw) dw = tdw;
        }
        if(dw > 1280) dw = 1280;
        s32 dh = ely + elemh + 30;
        if(dh > 720) dh = 720;
        s32 dx = (1280 - dw) / 2;
        s32 dy = (720 - dh) / 2;
        ely += dy;
        s32 elemw = ((dw - (20 * (this->optionTextures.size() + 1))) / this->optionTextures.size());
        s32 elx = dx + ((dw - ((elemw * this->optionTextures.size()) + (20 * (this->optionTextures.size() - 1)))) / 2);
        s32 r = 35;
        s32 nr = 180;
        s32 ng = 180;
        s32 nb = 200;
        bool end = false;
        s32 initfact = 0;
        while(true)
        {
            bool ok = ((Application*)App)->CallForRenderWithRenderOver([&](render::Renderer::Ref &Drawer) -> bool
            {
                u64 k = hidKeysDown(CONTROLLER_P1_AUTO);
                u64 h = hidKeysHeld(CONTROLLER_P1_AUTO);
                if((k & KEY_DLEFT) || (k & KEY_LSTICK_LEFT) || (h & KEY_RSTICK_LEFT))
                {
                    if(this->selectedOption > 0)
                    {
                        this->prevosel = this->selectedOption;
                        this->selectedOption--;
                        for(s32 i = 0; i < this->optionTextures.size(); i++)
                        {
                            if(i == this->selectedOption) this->selfact = 0;
                            else if(i == this->prevosel) this->pselfact = 255;
                        }
                    }
                }
                else if((k & KEY_DRIGHT) || (k & KEY_LSTICK_RIGHT) || (h & KEY_RSTICK_RIGHT))
                {
                    if(this->selectedOption < (this->optionTextures.size() - 1))
                    {
                        this->prevosel = this->selectedOption;
                        this->selectedOption++;
                        for(s32 i = 0; i < this->optionTextures.size(); i++)
                        {
                            if(i == this->selectedOption) this->selfact = 0;
                            else if(i == this->prevosel) this->pselfact = 255;
                        }
                    }
                }
                else if(k & KEY_A)
                {
                    this->cancel = false;
                    end = true;
                }
                else if(k & KEY_B)
                {
                    this->cancel = true;
                    end = true;
                }
                else if(hidKeysDown(CONTROLLER_HANDHELD) & KEY_TOUCH)
                {
                    touchPosition tch;
                    hidTouchRead(&tch, 0);
                    for(s32 i = 0; i < this->optionTextures.size(); i++)
                    {
                        String txt = this->options[i];
                        s32 rx = elx + ((elemw + 20) * i);
                        s32 ry = ely;
                        if(((rx + elemw) > tch.px) && (tch.px > rx) && ((ry + elemh) > tch.py) && (tch.py > ry))
                        {
                            this->selectedOption = i;
                            this->cancel = false;
                            end = true;
                        }
                    }
                }
                s32 bw = dw;
                s32 bh = dh;
                s32 fw = bw - (r * 2);
                s32 fh = bh - (r * 2);
                Color clr = { 225, 225, 225, initfact };
                s32 aclr = initfact;
                if(aclr < 0) aclr = 0;
                if(aclr > 125) aclr = 125;
                Drawer->RenderRectangleFill({ 0, 0, 0, (u8)aclr }, 0, 0, 1280, 720);
                Drawer->RenderRoundedRectangleFill(clr, dx, dy, bw, bh, r);
                render::SetAlphaValue(this->titleTexture, initfact);
                render::SetAlphaValue(this->contentTexture, initfact);
                Drawer->RenderTexture(this->titleTexture, (dx + 45), (dy + 55));
                Drawer->RenderTexture(this->contentTexture, (dx + 45), (dy + 140));
                if(this->hasIcon)
                {
                    s32 icw = render::GetTextureWidth(this->iconTexture);
                    s32 icx = dx + (dw - (icw + icm));
                    s32 icy = dy + icm;
                    Drawer->RenderTexture(this->iconTexture, icx, icy, { initfact, -1, -1, -1.0f });
                }
                for(s32 i = 0; i < this->optionTextures.size(); i++)
                {
                    s32 tw = render::GetTextureWidth(optionTextures[i]);
                    s32 th = render::GetTextureHeight(optionTextures[i]);
                    s32 tx = elx + ((elemw - tw) / 2) + ((elemw + 20) * i);
                    s32 ty = ely + ((elemh - th) / 2);
                    s32 rx = elx + ((elemw + 20) * i);
                    s32 ry = ely;
                    s32 rr = (elemh / 2);
                    Color dclr = { nr, ng, nb, initfact };
                    if(this->selectedOption == i)
                    {
                        if(this->selfact < 255)
                        {
                            dclr = { nr, ng, nb, this->selfact };
                            Drawer->RenderRoundedRectangleFill(dclr, rx, ry, elemw, elemh, rr);
                            this->selfact += 48;
                        }
                        else
                        {
                            dclr = { nr, ng, nb, initfact };
                            Drawer->RenderRoundedRectangleFill(dclr, rx, ry, elemw, elemh, rr);
                        }
                    }
                    else if(this->prevosel == i)
                    {
                        if(this->pselfact > 0)
                        {
                            dclr = { nr, ng, nb, this->pselfact };
                            Drawer->RenderRoundedRectangleFill(dclr, rx, ry, elemw, elemh, rr);
                            this->pselfact -= 48;
                        }
                    }
                    render::SetAlphaValue(this->optionTextures[i], initfact);
                    Drawer->RenderTexture(this->optionTextures[i], tx, ty);
                }
                if(end)
                {
                    if(initfact == 0) return false;
                    if(initfact > 0) initfact -= 25;
                    if(initfact < 0) initfact = 0;
                }
                else
                {
                    if(initfact < 255) initfact += 25;
                    if(initfact > 255) initfact = 255;
                }
                return true;
            });
            if(!ok)
            {
                ((Application*)App)->CallForRenderWithRenderOver([&](render::Renderer::Ref &Drawer) -> bool {});
                break;
            }
        }
        return this->selectedOption;
    }

    bool Dialog::UserCancelled()
    {
        return this->cancel;
    }

    bool Dialog::IsOk()
    {
        bool ok = true;
        if(this->cancel) ok = false;
        if(this->hcancel && (this->selectedOption == (this->optionTextures.size() - 1))) ok = false;
        return ok;
    }
}