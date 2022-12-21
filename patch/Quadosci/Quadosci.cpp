#include "daisy_patch.h"
#include "daisysp.h"
#include <string>

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;

Oscillator osc[4];
int waveforms[4] = { 0, 0, 0, 0 };
bool screensaverIsOn = false;

std::string waveNames[5];

int finalWave;
int currentOsc;
int idleSinceMs;
bool oscIsActive;

void SetupOsc(float);
void SetupWaveNames();
void AudioCallback(AudioHandle::InputBuffer, AudioHandle::OutputBuffer, size_t);
void UpdateOled();
void UpdateControls();
void PostponeScreensaver();
void StartScreensaver();

int main(void)
{
	float samplerate;
  	patch.Init();
  	samplerate = patch.AudioSampleRate();

    currentOsc = 0;
    oscIsActive = false;
    idleSinceMs = System::GetNow();
  	finalWave = Oscillator::WAVE_POLYBLEP_TRI;

  	SetupOsc(samplerate);
  	SetupWaveNames();

  	patch.StartAdc();
  	patch.StartAudio(AudioCallback);
  	while(1)
  	{
        UpdateOled();
  	}
}

void SetupOsc(float samplerate)
{
    for(int i = 0; i < 4; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetAmp(.7);
    }
}

void SetupWaveNames()
{
    waveNames[0] = "sine";
    waveNames[1] = "triangle";
    waveNames[2] = "saw";
    waveNames[3] = "ramp";
    waveNames[4] = "square";
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  	UpdateControls();
  	for(size_t i = 0; i < size; i++)
  	{
        //Process and output the four oscillators
        for(size_t chn = 0; chn < 4; chn++)
        {
            float sig = osc[chn].Process();
            out[chn][i] = sig;
        }
  	}
}

void UpdateOled()
{
    patch.display.Fill(false);

    if (screensaverIsOn) {
        patch.display.Update();
        return;
    }

    patch.display.SetCursor(0, 0);
    std::string title  = "Quadosci";
    char* ctitle = &title[0];
    patch.display.WriteString(ctitle, Font_7x10, true);

    std::string osc = "Osc 1";
    char* cosc = &osc[0];
    patch.display.SetCursor(10, 12);
    patch.display.WriteString(cosc, Font_7x10, true);

    char* cwav = &waveNames[waveforms[0]][0];
    patch.display.SetCursor(60, 12);
    patch.display.WriteString(cwav, Font_7x10, true);

    osc = "Osc 2";
    patch.display.SetCursor(10, 24);
    patch.display.WriteString(cosc, Font_7x10, true);

    cwav = &waveNames[waveforms[1]][0];
    patch.display.SetCursor(60, 24);
    patch.display.WriteString(cwav, Font_7x10, true);

    osc = "Osc 3";
    patch.display.SetCursor(10, 36);
    patch.display.WriteString(cosc, Font_7x10, true);

    cwav = &waveNames[waveforms[2]][0];
    patch.display.SetCursor(60, 36);
    patch.display.WriteString(cwav, Font_7x10, true);

    osc = "Osc 4";
    patch.display.SetCursor(10, 48);
    patch.display.WriteString(cosc, Font_7x10, true);

    cwav = &waveNames[waveforms[3]][0];
    patch.display.SetCursor(60, 48);
    patch.display.WriteString(cwav, Font_7x10, true);

    for(int i = 0; i < 4; i++)
    {
        osc = (currentOsc == i) ? ">" : "-";
        patch.display.SetCursor(0, (i + 1) * 12 - 1);
        patch.display.WriteString(cosc, Font_7x10, true);
    }

    for(int i = 0; i < 4; i++)
    {
        osc = (oscIsActive && currentOsc == i) ? ">" : ":";
        patch.display.SetCursor(48, (i + 1) * 12);
        patch.display.WriteString(cosc, Font_7x10, true);
    }

    patch.display.Update();
}

void UpdateControls()
{
    patch.ProcessDigitalControls();
    patch.ProcessAnalogControls();

    //knobs
    float ctrl[4];
    for(int i = 0; i < 4; i++)
    {
        ctrl[i] = patch.GetKnobValue((DaisyPatch::Ctrl)i);
    }

    for(int i = 0; i < 4; i++)
    {
        ctrl[i] = ctrl[i] * 5.f;           //voltage
        ctrl[i] = powf(2.f, ctrl[i]) * 55; //Hz
    }

    // save the current value of oscIsActive, currentOsc and waveforms[currentOsc]
    int waveformBefore = waveforms[currentOsc];
    bool oscIsActiveBefore = oscIsActive;
    int currentOscBefore = currentOsc;

    if(oscIsActive)
    {
        waveforms[currentOsc] += patch.encoder.Increment();
        waveforms[currentOsc] = (waveforms[currentOsc] % finalWave + finalWave) % finalWave;
        oscIsActive = patch.encoder.RisingEdge() ? false : true;
    }
    else
    {
        currentOsc += patch.encoder.Increment();
        currentOsc = (currentOsc % 4 + 4) % 4;
        oscIsActive = patch.encoder.RisingEdge() ? true : false;
    }

    // postpone or activate screensaver
    if(
        (waveformBefore != waveforms[currentOsc]) ||
        (oscIsActiveBefore != oscIsActive) ||
        (currentOscBefore != currentOsc)
    ) PostponeScreensaver();
    else if((System::GetNow() - idleSinceMs) > 1000 * 60 * 1) StartScreensaver();

    //Adjust oscillators based on inputs
    for(int i = 0; i < 4; i++)
    {
        osc[i].SetFreq(ctrl[i]);
        osc[i].SetWaveform((uint8_t)waveforms[i]);
    }
}

void StartScreensaver() {
    oscIsActive = false;
    screensaverIsOn = true;
}

void PostponeScreensaver() {
    screensaverIsOn = false;
    idleSinceMs = System::GetNow();
}