# Quadosci

## Author

Don Smith

## Description

Four different oscillators with individually assignable waveforms with 1v/o pitch control each! Includes a screensaver.

[Source Code](https://github.com/don-smith/DaisyExamples/tree/main/patch/Quadosci)

## Controls

| Control | Description | Comment |
| --- | --- | --- |
| Ctrl 1 - 4 | Oscillator Frequency | Manually controls the pitch of individual oscillators |
| CV 1 - 4 | Oscillator Frequency | Controls the pitch of individual oscillators 1 volt/octave |
| Encoder | Waveform | Push to select channel. Rotate to cycle through sine, triangle, saw, ramp, and square |
| Audio Out 1 - 4 | Oscillator Output | Outputs for individual oscillators |

## Screensaver

Includes a [screensaver built by Michael Serra](https://github.com/mkserra/DaisyExamples/tree/master/patch/mserra/screensavers) that will start after 3 minutes. When it starts, it will deselect any selected channels so you can turn the encoder to disable the screensaver without changing the waveform of the last selected channel. Pushing the encoder with the screensaver on will turn it off and reselect the last selected channel. Manually adjusting pitch will not disable the screensaver - only depressing or rotating the encoder.

## Notes

Largely borrowed from the PolyOsc example with a different UI.
