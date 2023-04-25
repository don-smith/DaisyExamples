#include "daisy_patch.h"
#include "daisysp.h"
#include "voices.h"
#include <string>

DaisyPatch patch;

size_t waveforms[4] = {
    Oscillator::WAVE_SIN,
    Oscillator::WAVE_TRI,
    Oscillator::WAVE_SAW,
    Oscillator::WAVE_SQUARE
};

// Limit wave shapes to sine, triangle, saw, ramp & square
size_t lastwave = Oscillator::WAVE_POLYBLEP_TRI;
std::string waveNames[5] = {
    "sin", "tri", "saw", "rmp", "sqr"
};

VoiceManager<8> managers[4];

void SetupVoiceManagers(float);
void HandleMidiMessage(daisy::MidiEvent);
void AudioCallback(AudioHandle::InputBuffer, AudioHandle::OutputBuffer, size_t);
void UpdateOled();

int main(void)
{
	float samplerate;

	patch.Init();
	samplerate = patch.AudioSampleRate();
	SetupVoiceManagers(samplerate);

	patch.midi.StartReceive();
	patch.StartAdc();
	patch.StartAudio(AudioCallback);

	while(1) {
        UpdateOled();
        patch.midi.Listen();
        // Handle MIDI Events
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
        VoiceManager<8> *mgr = &managers[i];
        mgr->Init(samplerate, waveforms[i]);
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float sum = 0.f;

	// patch.ProcessAllControls();

	for (size_t i = 0; i < size; i++)
	{
	    //Process and output the four oscillators
        for(size_t chn = 0; chn < 4; chn++)
        {
			sum = 0.f;
            VoiceManager<8> *mgr = &managers[chn];
			sum = mgr->Process() * 0.5f;
			out[chn][i] = sum;
        }
	}
}

// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    VoiceManager<8> *mgr = &managers[m.channel];

    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            // Note Off can come in as Note On w/ 0 Velocity
            if(p.velocity == 0.f)
            {
                mgr->OnNoteOff(p.note);
            }
            else
            {
                mgr->OnNoteOn(p.note, p.velocity);
            }
        }
        break;
        case NoteOff:
        {
            NoteOffEvent p = m.AsNoteOff();
            mgr->OnNoteOff(p.note);
        }
        break;
        default: break;
    }
}

void UpdateOled() {
    size_t mgr_count = 4;
    patch.display.Fill(false);

    for(size_t i = 0; i < mgr_count; i++) {
        VoiceManager<8> *mgr = &managers[i];
        size_t row = i * 12, col = 0;
        patch.display.SetCursor(col, row);
        size_t active = mgr->GetActiveCount();
        std::string count = std::to_string(active);
        patch.display.WriteString(&count[0], Font_7x10, true);
    }

    patch.display.Update();
}
