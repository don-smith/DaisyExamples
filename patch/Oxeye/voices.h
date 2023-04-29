#pragma once

#include "daisy_patch.h"
#include "daisysp.h"

namespace oxeye
{
  class Voice
  {
    public:
      Voice();
      ~Voice();

      void  Init(float samplerate, size_t waveform);
      void  OnControlChange(size_t number, float value);
      void  OnNoteOn(float note, float velocity);
      void  OnNoteOff();
      float Process();

      inline bool  IsActive() const;
      inline float GetNote()  const;

    private:
      daisysp::Oscillator osc_;
      daisysp::Adsr       env_;
      daisysp::Svf        filt_;
      float               note_, velocity_;
      bool                active_, env_gate_;
  };

  template<size_t max_voices>
  class VoiceManager
  {
    public:
      VoiceManager();
      ~VoiceManager();

      void  Init(float samplerate, size_t waveform);
      void  OnControlChange(float number, float value);
      void  OnNoteOn(float notenumber, float velocity);
      void  OnNoteOff(float notenumber);
      float Process();

      size_t GetActiveCount();

    private:
      Voice  voices[max_voices];
      Voice* FindFreeVoice();
  };
}