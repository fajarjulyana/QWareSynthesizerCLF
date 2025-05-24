/*
  ==============================================================================

    SynthSound.h
    Created: 21 May 2025 12:18:27am
    Author:  fajarjulyana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};
