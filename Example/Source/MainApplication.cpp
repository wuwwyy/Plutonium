#include <MainApplication.hpp>

MainApplication* mainApp;

// Implement all the layout/application functions here
using namespace pu::ui::elm;
CustomLayout::CustomLayout() : Layout::Layout()
{
    this->helloText = TextBlock::New(0, 0, "A", 300);
    Menu::Ref menu = Menu::New(0,0,1280,pu::ui::Color(0xAF,0,0,0xFF),72,10);
    menu->SetOnFocusColor(pu::ui::Color(0xFF,0,0,0xFF));
    auto itm = MenuItem::New("dick");
    menu->AddItem(itm);
    menu->AddItem(itm);
    menu->AddItem(itm);
    this->Add(menu);
    this->Add(this->helloText);
}

void CustomLayout::OnInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos)
{
    if(Down & KEY_X) // If X is pressed, start with our dialog questions!
    {
        int opt = mainApp->CreateShowDialog("Warning!", "Due to the nature of libnx's USB comms implementation, USB installations may not \"just werk\" on some devices and setups. If you experience issues with USB installations, please don't pull your hair out! It's advised to use LAN/Internet installations instead for remote installation, especially when paired with an ethernet adapter! It is not recomended, but disabling NCA verification in Awoo Installer's settings may help if you plan to use USB installations a lot.\n\nThis is not Awoo Installer's fault, I promise. You have been warned...", { "A again", "B", "Cancel" }, true, 850); // (using latest option as cancel option)
        if (opt == 0) mainApp->LoadLayout(CustomLayout::New());
        else if (opt == 1) mainApp->LoadLayout(BLayout::New());
    }
    else if(Down & KEY_B)
    {
        mainApp->PopLayout();
    }
    else if(Down & KEY_PLUS) // If + is pressed, exit application
    {
        mainApp->Close();
    }
}

bool CustomLayout::OnClose() {
    return (mainApp->CreateShowDialog("Close?", "Do you really want to close the application?", { "Yes", "No" }, true) == 0);
}

void CustomLayout::OnTick() {}

BLayout::BLayout() : Layout::Layout()
{
    this->helloText = TextBlock::New(0, 0, "B", 300);
    this->Add(this->helloText);
}

void BLayout::OnInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos)
{
    if(Down & KEY_B)
    {
        mainApp->PopLayout();
    }
    else if(Down & KEY_PLUS) // If + is pressed, exit application
    {
        mainApp->Close();
    }
}

bool BLayout::OnClose() {
    return true;
}

void BLayout::OnTick() {
    this->count++;
    this->helloText->SetText(std::to_string(count));
}

void MainApplication::OnLoad()
{
    mainApp = this;

    // Load the layout. In applications layouts are loaded, not added into a container (you don't select an added layout, just load it from this function)
    // Simply explained: loading layout = the application will render that layout in the very next frame
    this->LoadLayout(CustomLayout::New());
}