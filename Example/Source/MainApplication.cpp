#include <MainApplication.hpp>

MainApplication* mainApp;

// Implement all the layout/application functions here
using namespace pu::ui::elm;
CustomLayout::CustomLayout() : Layout::Layout()
{
    this->helloText = TextBlock::New(0, 0, "A", 300);
    this->Add(this->helloText);
}

void CustomLayout::OnInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos)
{
    if(Down & KEY_X) // If X is pressed, start with our dialog questions!
    {
        int opt = mainApp->CreateShowDialog("Load", "Do you like apples?", { "A again", "B", "Cancel" }, true); // (using latest option as cancel option)
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
    return false;
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