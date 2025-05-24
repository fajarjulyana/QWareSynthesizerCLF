#include "PluginEditor.h"
#include "PluginProcessor.h"

SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p),
      keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setLookAndFeel(&qwareLookAndFeel);
    setSize(600, 400);
   // Attack slider
    attackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    attackSlider.setRange(0.0, 1.0, 0.01);
    attackSlider.setName("Attack");
    attackSlider.setTextValueSuffix("");
    addAndMakeVisible(attackSlider);
   
    // decaySlider
    decaySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    decaySlider.setRange(0.0, 1.0, 0.01);
    decaySlider.setName("Decay");
    decaySlider.setTextValueSuffix("");
    addAndMakeVisible(decaySlider);
   
    // sustainSlider
    sustainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    sustainSlider.setRange(0.0, 1.0, 0.01);
    sustainSlider.setName("Sustain");
    sustainSlider.setTextValueSuffix("");
    addAndMakeVisible(sustainSlider);
   
    // releaseSlider
    releaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    releaseSlider.setRange(0.0, 1.0, 0.01);
    releaseSlider.setName("Release");
    releaseSlider.setTextValueSuffix("");
    addAndMakeVisible(releaseSlider);
   
    // lfoRateSlider
    lfoRateSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfoRateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    lfoRateSlider.setRange(0.0, 1.0, 0.01);
    lfoRateSlider.setName("LFO Rate");
    lfoRateSlider.setTextValueSuffix("");
    addAndMakeVisible(lfoRateSlider);
   
    // lfoRateSlider
    lfoDepthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfoDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    lfoDepthSlider.setRange(0.0, 1.0, 0.01);
    lfoDepthSlider.setName("LFO");
    lfoDepthSlider.setTextValueSuffix("");
    addAndMakeVisible(lfoDepthSlider);
// Attack label
attackLabel.setText("Attack", juce::dontSendNotification);
attackLabel.attachToComponent(&attackSlider, false); // false = label di bawah knob
attackLabel.setJustificationType(juce::Justification::centred);
addAndMakeVisible(attackLabel);

// Decay label
decayLabel.setText("Decay", juce::dontSendNotification);
decayLabel.attachToComponent(&decaySlider, false);
decayLabel.setJustificationType(juce::Justification::centred);
addAndMakeVisible(decayLabel);

// Lanjutkan untuk semua knob
sustainLabel.setText("Sustain", juce::dontSendNotification);
sustainLabel.attachToComponent(&sustainSlider, false);
sustainLabel.setJustificationType(juce::Justification::centred);
addAndMakeVisible(sustainLabel);

releaseLabel.setText("Release", juce::dontSendNotification);
releaseLabel.attachToComponent(&releaseSlider, false);
releaseLabel.setJustificationType(juce::Justification::centred);
addAndMakeVisible(releaseLabel);

lfoRateLabel.setText("LFO Rate", juce::dontSendNotification);
lfoRateLabel.attachToComponent(&lfoRateSlider, false);
lfoRateLabel.setJustificationType(juce::Justification::centred);
addAndMakeVisible(lfoRateLabel);

lfoDepthLabel.setText("LFO", juce::dontSendNotification);
lfoDepthLabel.attachToComponent(&lfoDepthSlider, false);
lfoDepthLabel.setJustificationType(juce::Justification::centred);
addAndMakeVisible(lfoDepthLabel);

    auto& apvts = processor.apvts;
    attackAttachment  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ATTACK",  attackSlider);
    decayAttachment   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "DECAY",   decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "RELEASE", releaseSlider);
    lfoRateAttachment  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "LFO_RATE",  lfoRateSlider);
    lfoDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "LFO_DEPTH", lfoDepthSlider);

    // Add keyboard
    addAndMakeVisible(keyboardComponent);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor() {
 setLookAndFeel(nullptr);
}

void SynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    // Beri ruang lebih tinggi untuk judul, misal 50 px
    g.drawFittedText("Fajar Julyana Synthesizer", getLocalBounds().removeFromTop(50),
                     juce::Justification::centred, 1);
}


void SynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    // Ambil ruang judul dulu supaya tidak tertimpa knob/label
    auto titleArea = area.removeFromTop(50);

    auto sliderArea = area.removeFromTop(200);

    // ADSR sliders (empat knob)
    auto adsrArea = sliderArea.removeFromLeft(300);

    int knobWidth = 70;
    int knobHeight = 100; // tinggi knob + label cukup lega

    attackSlider.setBounds(adsrArea.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(5));

    decaySlider.setBounds(adsrArea.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(5));

    sustainSlider.setBounds(adsrArea.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(5));

    releaseSlider.setBounds(adsrArea.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(5));

    // LFO sliders (dua knob)
    auto lfoArea = sliderArea;
    int lfoKnobWidth = 100;
    int lfoKnobHeight = 100;

    lfoRateSlider.setBounds(lfoArea.removeFromLeft(lfoKnobWidth).withHeight(lfoKnobHeight).reduced(10));

    lfoDepthSlider.setBounds(lfoArea.removeFromLeft(lfoKnobWidth).withHeight(lfoKnobHeight).reduced(10));

    // Keyboard di bawah semua
    keyboardComponent.setBounds(area.removeFromBottom(100));
}
