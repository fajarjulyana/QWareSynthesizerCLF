/*
  ==============================================================================

    SynthVoice.h
    Created: 21 May 2025 12:17:58am
    Author:  fajarjulyana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice() {}

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity,
               juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
{
    currentAngle = 0.0;
    level = velocity;
    tailOff = 0.0;

    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    angleDelta = frequency * juce::MathConstants<double>::twoPi / getSampleRate(); // <-- Tambahkan ini!
    adsr.noteOn();
}


    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            adsr.noteOff();
        }
        else
        {
            clearCurrentNote();
            adsr.reset();
        }
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    auto* leftBuffer  = outputBuffer.getWritePointer(0, startSample);
    auto* rightBuffer = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

    while (--numSamples >= 0)
    {
        auto lfo = std::sin(lfoPhase) * lfoDepth;
        lfoPhase += lfoIncrement;
        if (lfoPhase > juce::MathConstants<float>::twoPi)
            lfoPhase -= juce::MathConstants<float>::twoPi;

        auto sample = (float)(std::sin(currentAngle + lfo) * level);
        sample = std::tanh(sample); // wave shaper
        sample *= adsr.getNextSample();

        if (leftBuffer)  *leftBuffer++  += sample;
        if (rightBuffer) *rightBuffer++ += sample;

        currentAngle += angleDelta;
    }

    if (!adsr.isActive())
        clearCurrentNote();
}

    void setADSR(float attack, float decay, float sustain, float release)
    {
        adsrParams.attack = attack;
        adsrParams.decay = decay;
        adsrParams.sustain = sustain;
        adsrParams.release = release;
        adsr.setParameters(adsrParams);
    }

    void setLFO(float rate, float depth)
    {
        lfoRate = rate;
        lfoDepth = depth;
        lfoIncrement = lfoRate * juce::MathConstants<float>::twoPi / getSampleRate();
    }

private:
 double currentAngle = 0.0;
    double angleDelta = 0.0;
    double frequency = 440.0;
    float level = 0.0f;
    double tailOff = 0.0;
    // ADSR
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    // LFO
    float lfoRate = 5.0f;
    float lfoDepth = 5.0f;
    float lfoPhase = 0.0f;
    float lfoIncrement = 0.0f;
};
