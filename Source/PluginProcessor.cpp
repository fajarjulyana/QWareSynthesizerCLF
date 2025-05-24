#include "PluginProcessor.h"
#include "PluginEditor.h"

SynthAudioProcessor::SynthAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameters())
{
    for (int i = 0; i < 4; ++i)
        synth.addVoice(new SynthVoice());

    synth.addSound(new SynthSound());
}

void SynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

bool SynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // Get params from apvts
            auto attack  = apvts.getRawParameterValue("ATTACK")->load();
            auto decay   = apvts.getRawParameterValue("DECAY")->load();
            auto sustain = apvts.getRawParameterValue("SUSTAIN")->load();
            auto release = apvts.getRawParameterValue("RELEASE")->load();

            auto lfoRate  = apvts.getRawParameterValue("LFO_RATE")->load();
            auto lfoDepth = apvts.getRawParameterValue("LFO_DEPTH")->load();

            voice->setADSR(attack, decay, sustain, release);
            voice->setLFO(lfoRate, lfoDepth);
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    return new SynthAudioProcessorEditor(*this);
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK",  "Attack",  0.01f, 5.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY",   "Decay",   0.01f, 5.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 0.8f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.01f, 5.0f, 0.4f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_RATE",  "LFO Rate",  0.1f, 20.0f, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_DEPTH", "LFO Depth", 0.0f, 50.0f, 5.0f));

    return { params.begin(), params.end() };
}

// JUCE Plugin boilerplate
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}
