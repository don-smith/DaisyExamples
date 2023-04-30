#include <string>

#include "daisy_patch.h"
#include "daisysp.h"

#include "voices.h"
#include "ui/ui.h"

using namespace std;
using namespace daisy;
using namespace daisysp;

size_t waveforms[4] = {
    Oscillator::WAVE_SIN,
    Oscillator::WAVE_TRI,
    Oscillator::WAVE_SAW,
    Oscillator::WAVE_SQUARE
};

// Limit wave shapes to sine, triangle, saw, ramp & square
size_t lastwave = Oscillator::WAVE_POLYBLEP_TRI;
string waveNames[5] = {
    "sin", "tri", "saw", "rmp", "sqr"
};

static oxeye::VoiceManager<8> managers[4];

void SetupVoiceManagers(float);
void UpdateOled(DaisyPatch, oxeye::UI);
void HandleMidiMessage(daisy::MidiEvent);
void AudioCallback(AudioHandle::InputBuffer, AudioHandle::OutputBuffer, size_t);

int main(void)
{
    oxeye::UI ui;
	float samplerate;
    DaisyPatch patch;

	patch.Init();
    ui.Init(&patch);
	samplerate = patch.AudioSampleRate();
	SetupVoiceManagers(samplerate);

	patch.midi.StartReceive();
	patch.StartAdc();
	patch.StartAudio(AudioCallback);

	while(1) {
        ui.Render();

        patch.midi.Listen();
        while(patch.midi.HasEvents())
        {
            HandleMidiMessage(patch.midi.PopEvent());
        }
	}
}

void SetupVoiceManagers (float samplerate)
{
    for(size_t i = 0; i < 4; i++)
    {
        managers[i].Init(samplerate, waveforms[i]);
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float sum = 0.f;

	for (size_t i = 0; i < size; i++)
	{
	    //Process and output the four oscillators one per channel
        for(size_t chn = 0; chn < 4; chn++)
        {
			sum = 0.f;
			sum = managers[chn].Process() * 0.5f;
			out[chn][i] = sum;
        }
	}
}

// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    oxeye::VoiceManager<8> *mgr = &managers[m.channel];

    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent n = m.AsNoteOn();
            // Note Off can come in as Note On w/ 0 Velocity
            if(n.velocity == 0.f)
            {
                mgr->OnNoteOff(n.note);
            }
            else
            {
                mgr->OnNoteOn(n.note, n.velocity);
            }
        }
        break;

        case NoteOff:
        {
            mgr->OnNoteOff(m.AsNoteOff().note);
        }
        break;

        case ControlChange:
        {
            ControlChangeEvent cc = m.AsControlChange();
            mgr->OnControlChange(cc.control_number, cc.value);
        }
        break;

        default: break;
    }
}
