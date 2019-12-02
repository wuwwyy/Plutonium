
#pragma once

// Include Plutonium's main header
#include <pu/Plutonium>

// Define your main layout as a class inheriting from pu::Layout
class CustomLayout : public pu::ui::Layout
{
    public:

        CustomLayout();
        void OnInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos) override;
        void OnStart() override;
        bool OnStop() override;
        void OnTick() override;

        // Have ::Ref alias and ::New() static constructor
        PU_SMART_CTOR(CustomLayout)

    private:
        // An easy way to keep objects is to have them as private members
        // Using ::Ref (of a Plutonium built-in object or any class having PU_SMART_CTOR) is an alias to a shared_ptr of the instance.
        pu::ui::elm::TextBlock::Ref helloText;
        pu::ui::elm::Menu::Ref menu;
};

// Define your main layout as a class inheriting from pu::Layout
class BLayout : public pu::ui::Layout
{
    public:

        BLayout();
        void OnInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos) override;
        void OnStart() override;
        bool OnStop() override;
        void OnTick() override;

        // Have ::Ref alias and ::New() static constructor
        PU_SMART_CTOR(BLayout)

    private:
        int count = 0;
        // An easy way to keep objects is to have them as private members
        // Using ::Ref (of a Plutonium built-in object or any class having PU_SMART_CTOR) is an alias to a shared_ptr of the instance.
        pu::ui::elm::TextBlock::Ref helloText;
        pu::ui::elm::Menu::Ref menu;
};

// Define your application (can't instantiate base class, so need to make a derived one)
class MainApplication : public pu::ui::Application
{
    public:
        using Application::Application;
        PU_SMART_CTOR(MainApplication)

        // We need to define this, and use it to initialize everything
        void OnLoad() override;
};