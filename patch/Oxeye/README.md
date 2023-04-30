# Oxeye

## Author

Don Smith

## Description

MIDI controlled polyphonic 4-voice synth. Output channels can each use the same oscillator, or different ones. Each output has 8-note polyphony and can be configured to each listen on different MIDI channels. MIDI CC mapping for segment of the ADSR envelope as well as cutoff, resonance and drive for the state variable filter.

**Currently still working on the menu system** so it cannot yet be configured.

Default output mappings (despite what the menu displays):

| Output channel | Oscillator | MIDI channel |
|----------------|------------|--------------|
| 1              | Sine       | 1            |
| 2              | Triangle   | 2            |
| 3              | Saw        | 3            |
| 4              | Square     | 4            |

And here are the default MIDI CC mappings:

| Parameter         | MIDI CC |
|-------------------|---------|
| Envelope: Attack  | 73      |
| Envelope: Decay   | 80      |
| Envelope: Sustain | 7       |
| Envelope: Release | 72      |
| Filter: Cutoff    | 74      |
| Filter: Resonance | 71      |
| Filter: Drive     | 77      |
| Detune (future)   | 94      |

These will all be configurable once the menu UI is done.
