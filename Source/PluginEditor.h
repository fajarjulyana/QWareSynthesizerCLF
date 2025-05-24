#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "QWareLookAndFeel.h" // Ganti dari CustomLookAndFeel.h

class SynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SynthAudioProcessorEditor(SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SynthAudioProcessor& processor;
    QWareLookAndFeel qwareLookAndFeel;
    // === GUI Controls ===
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider,lfoRateSlider, lfoDepthSlider;
  
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel, lfoRateLabel, lfoDepthLabel;

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    // === Attachments ===
 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateAttachment;
 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoDepthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor)
};
