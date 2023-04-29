// #pragma once

#include "daisy_patch.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class Voice
{
  public:
    Voice();
    ~Voice();

    void Init(float samplerate, size_t waveform);
    float Process();
    void OnNoteOn(float note, float velocity);
    void OnNoteOff();

    inline bool  IsActive() const;
    inline float GetNote() const;

  private:
    Oscillator osc_;
    Adsr       env_;
    float      note_, velocity_;
    bool       active_, env_gate_;
};

template<size_t max_voices>
class VoiceManager
{
  public:
    VoiceManager();
    ~VoiceManager();

    float Process();
    void Init(float samplerate, size_t waveform);
    void OnNoteOn(float notenumber, float velocity);
    void OnNoteOff(float notenumber);

    size_t GetActiveCount();

  private:
    Voice  voices[max_voices];
    Voice* FindFreeVoice();
};
