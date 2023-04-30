#include <string>

#include "ui.h"
#include "home.h"

using namespace std;
using namespace daisy;

oxeye::UI::UI(){}
oxeye::UI::~UI(){}

void oxeye::UI::Init(DaisyPatch* patch)
{
    patch_ = patch;
}

void oxeye::UI::Render()
{
    // identify current screen (global variable?) eventually this will be done by nav
    HomeController home;
    // subscribe controller functions to events
    DispatchEvents();

    string view[5];
    home.GetView(view);

    patch_->display.Fill(false);

    for(size_t i = 0; i < LAST_DISPLAY_ROW; i++)
    {
        string rowcontents = view[i];
        patch_->display.SetCursor(0, i * ROW_HEIGHT);
        patch_->display.WriteString(&rowcontents[0], Font_7x10, true);
    }

    patch_->display.Update();
}

void oxeye::UI::DispatchEvents()
{
    patch_->ProcessDigitalControls();
    patch_->ProcessAnalogControls();

    int32_t lastval = patch_->encoder.Increment();

    int32_t currentval = patch_->encoder.Increment();
    if(lastval != currentval)
    {
        // for(size_t i = 0; i < subscribers.length i++;)
        // {
        //     // pass current value to subscribers
        // }
    }
    lastval = currentval;

    if(patch_->encoder.RisingEdge())
    {
        // for(size_t i = 0; i < subscribers.length i++;)
        // {
        //     // notify subscribers
        // }
    }
}
