#include "Voices.h"

//
// Voice implementation
//

Voice::Voice() {}
Voice::~Voice() {}

void Voice::Init(float samplerate, size_t waveform)
{
    note_ = 0.f;
    velocity_ = 0.f;
    active_ = false;
    env_gate_ = false;
    osc_.Init(samplerate);
    osc_.SetAmp(0.75f);
    osc_.SetWaveform(waveform);
    env_.Init(samplerate);
    env_.SetSustainLevel(0.5f);
    env_.SetTime(ADSR_SEG_ATTACK, 0.005f);
    env_.SetTime(ADSR_SEG_DECAY, 0.005f);
    env_.SetTime(ADSR_SEG_RELEASE, 0.2f);
    filt_.Init(samplerate);
    filt_.SetFreq(6000.f);
    filt_.SetRes(0.6f);
    filt_.SetDrive(0.8f);
}

float Voice::Process()
{
    if(active_)
    {
        float sig = 0, amp = 0;
        amp = env_.Process(env_gate_);
        active_ = env_.IsRunning();
        sig = osc_.Process();
        filt_.Process(sig);
        return filt_.Low() * (velocity_ / 127.f) * amp;
    }
    return 0.f;
}

void Voice::OnNoteOn(float note, float velocity)
{
    note_     = note;
    velocity_ = velocity;
    osc_.SetFreq(mtof(note_));
    env_gate_ = active_ = true;
}

void Voice::OnControlChange(size_t number, float value)
{
    switch(number)
    {
        case 73: // Attack
        {
            env_.SetTime(ADSR_SEG_ATTACK, value / 127.f);
        }
        break;

        case 80: // Decay
        {
            env_.SetTime(ADSR_SEG_DECAY, value / 127.f);
        }
        break;

        case 7: // Sustain
        {
            env_.SetSustainLevel(value / 127.f);
        }
        break;

        case 72: // Release
        {
            env_.SetTime(ADSR_SEG_RELEASE, value / 127.f);
        }
        break;

        case 74: // Cutoff
        {
            filt_.SetFreq(mtof(value));
        }
        break;

        case 71: // Resonance
        {
            filt_.SetRes(value / 127.f);
        }
        break;

        case 94: // Detune
        {
            // filt_.SetDrive(value / 127.f);
        }
        break;

        case 77: // Drive
        {
            filt_.SetDrive(value / 127.f);
        }
        break;

        default: break;
    }
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
void VoiceManager<max_voices>::OnControlChange(float number, float value)
{
    for(size_t i = 0; i < max_voices; i++)
    {
        voices[i].OnControlChange(number, value);
    }
}

template<size_t max_voices>
void VoiceManager<max_voices>::OnNoteOn(float notenumber, float velocity)
{
    Voice *v = FindFreeVoice();
    if(v != NULL)
    {
        v->OnNoteOn(notenumber, velocity);
    }
}

template<size_t max_voices>
void VoiceManager<max_voices>::OnNoteOff(float notenumber)
{
    for(size_t i = 0; i < max_voices; i++)
    {
        Voice *v = &voices[i];
        if(v->IsActive() && v->GetNote() == notenumber)
        {
            v->OnNoteOff();
        }
    }
}

template<size_t max_voices>
Voice* VoiceManager<max_voices>::FindFreeVoice()
{
    for(size_t i = 0; i < max_voices; i++)
    {
        if(!voices[i].IsActive())
        {
            return &voices[i];
        }
    }
    return NULL;
}

template<size_t max_voices>
size_t VoiceManager<max_voices>::GetActiveCount()
{
    size_t count = 0;
    for(size_t i = 0; i < max_voices; i++)
    {
        if(voices[i].IsActive())
        {
            count += 1;
        }
    }
    return count;
}

template class VoiceManager<8>;
