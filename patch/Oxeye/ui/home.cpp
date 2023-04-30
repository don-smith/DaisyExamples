#include <map>
#include <string>

#include "daisysp.h"
#include "home.h"

using namespace std;
using namespace daisysp;

std::map<size_t, std::string> oscillatorNames
{
    { daisysp::Oscillator::WAVE_SIN,             "Sine" },
    { daisysp::Oscillator::WAVE_TRI,             "Triangle" },
    { daisysp::Oscillator::WAVE_SAW,             "Saw" },
    { daisysp::Oscillator::WAVE_RAMP,            "Ramp" },
    { daisysp::Oscillator::WAVE_SQUARE,          "Square" },
    { daisysp::Oscillator::WAVE_POLYBLEP_TRI,    "PolyBlepTri" },
    { daisysp::Oscillator::WAVE_POLYBLEP_SAW,    "PolyBlepSaw" },
    { daisysp::Oscillator::WAVE_POLYBLEP_SQUARE, "PolyBlepSqr" },
};

class HomeView {
  public:
    HomeView(){}
    ~HomeView(){}

    void Render(HomeModel model, string* view)
    {
        view[0] = " Oxeye        v0.3";
        // one line for each of the 4 channels
        for(size_t i = 0; i < LAST_CHANNEL; i++)
        {
            size_t id = model.channels[i].oscillator;
            size_t midi = model.channels[i].midiChannel;
            string osc = oscillatorNames[id];
            string cursor = model.selectedChannel == i ? ">" : " ";
            view[i+1] = cursor + to_string(i+1) + ": " + &osc[0] + ":" + to_string(midi+1);
        }
    }
};

HomeController::HomeController(){};
HomeController::~HomeController(){};

void HomeController::GetView(string* view)
{
    HomeModel model;
    model.selectedChannel = 0;
    // { oscillator, midi channel} zero-based
    model.channels[0] = { 0, 2};
    model.channels[1] = { 1, 3};
    model.channels[2] = { 2, 9};
    model.channels[3] = { 4, 11};

    HomeView homeView;
    homeView.Render(model, view);
}
