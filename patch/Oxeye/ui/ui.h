#pragma once

#include "daisy_patch.h"

static const size_t ROW_HEIGHT = 13;
static const size_t LAST_DISPLAY_ROW = 5;

namespace oxeye {

    class UI
    {
      public:
        UI();
        ~UI();

        void Render();
        void Init(daisy::DaisyPatch* patch);

      private:
        daisy::DaisyPatch* patch_;

        void DispatchEvents();
    };

}
