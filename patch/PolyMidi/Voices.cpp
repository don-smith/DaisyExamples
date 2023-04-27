#include "Voices.h"

//
// Voice implementation
//

Voice::Voice() {}
Voice::~Voice() {}

void Voice::Init(float samplerate, size_t waveform)
{
    note_ = 0;
    velocity_ = 0;
    active_ = false;
    env_gate_ = false;
    osc_.Init(samplerate);
    osc_.SetAmp(0.75f);
    osc_.SetWaveform(waveform);
    env_.Init(samplerate);
    env_.SetSustainLevel(0.5f);
    env_.SetTime(ADSR_SEG_ATTACK, 0.005f);
    env_.SetTime(ADSR_SEG_DECAY, 0.005f);
    env_.SetTime(ADSR_SEG_RELEASE, 0.02f);
}

float Voice::Process()
{
    if(active_)
    {
        float sig, amp;
        amp = env_.Process(env_gate_);
        active_ = env_.IsRunning();
        sig = osc_.Process();
        return sig * (velocity_ / 127.f) * amp;
    }
    return 0.f;
}

void Voice::OnNoteOn(float note, float velocity)
{
    note_     = note;
    velocity_ = velocity;
    env_gate_ = active_ = true;
    osc_.SetFreq(mtof(note_));
}

void Voice::OnNoteOff() { env_gate_ = false; }

inline bool Voice::IsActive() const { return active_; }

inline float Voice::GetNote() const { return note_; }

//
// VoiceManager implementation
//

template<size_t max_voices>
VoiceManager<max_voices>::VoiceManager() {}
template<size_t max_voices>
VoiceManager<max_voices>::~VoiceManager() {}

template<size_t max_voices>
void VoiceManager<max_voices>::Init(float samplerate, size_t waveform)
{
    for(size_t i = 0; i < max_voices; i++)
    {
        voices[i].Init(samplerate, waveform);
    }
}

template<size_t max_voices>
float VoiceManager<max_voices>::Process()
{
    float sum = 0.f;
    for(size_t i = 0; i < max_voices; i++)
    {
        sum += voices[i].Process();
    }
    return sum;
}

template<size_t max_voices>
void VoiceManager<max_voices>::OnNoteOn(float notenumber, float velocity)
{
    Voice *v = FindFreeVoice();
    if(v == NULL) {
        return;
    }
    v->OnNoteOn(notenumber, velocity);
}

template<size_t max_voices>
void VoiceManager<max_voices>::OnNoteOff(float notenumber)
{
    for(size_t i = 0; i < max_voices; i++)
    {
        Voice v = voices[i];
        if(v.IsActive() && v.GetNote() == notenumber)
        {
            v.OnNoteOff();
        }
    }
}

template<size_t max_voices>
void VoiceManager<max_voices>::FreeAllVoices()
{
    for(size_t i = 0; i < max_voices; i++)
    {
        voices[i].OnNoteOff();
    }
}

template<size_t max_voices>
Voice* VoiceManager<max_voices>::FindFreeVoice()
{
    Voice *found = NULL;
    for(size_t i = 0; i < max_voices; i++)
    {
        Voice *v = &voices[i];
        if(!v->IsActive())
        {
            found = v;
            break;
        }
    }
    return found;
}

template class VoiceManager<8>;
