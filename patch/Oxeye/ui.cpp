#include <string>

#include "daisy_patch.h"

#include "ui.h"

using namespace std;
using namespace daisy;

oxeye::UI::UI(){}
oxeye::UI::~UI(){}

void oxeye::UI::Show(DaisyPatch patch)
{
    patch.display.Fill(false);

    patch.display.SetCursor(0, 0);
    string title = " Oxeye        v0.1";
    patch.display.WriteString(&title[0], Font_7x10, true);

    string channels[4] { "Sine:3", "Triangle:4", "Saw:10", "Square:12" };

    for(size_t i = 1; i < 5; i++)
    {
        string chn = channels[i-1];
        size_t row = i * 13, col = 0;
        patch.display.SetCursor(col, row);
        string channel = ">" + to_string(i) + ": " + &chn[0];
        patch.display.WriteString(&channel[0], Font_7x10, true);
    }

    patch.display.Update();
}
