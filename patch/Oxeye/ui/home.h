#pragma once

#include <map>
#include <string>

#include "daisysp.h"

static const size_t LAST_CHANNEL = 4;

struct Channel {
    size_t oscillator;      // 0-7
    size_t midiChannel;     // 0-15
};

struct HomeModel {
   Channel channels[4];     // 0-3
   size_t  selectedChannel; // 0-3
};

class HomeController {
  public:
    HomeController();
    ~HomeController();

    void GetView(std::string*);
};
