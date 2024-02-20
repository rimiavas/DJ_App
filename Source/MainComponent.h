#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent , public juce::Button::Listener, public juce::Slider::Listener

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;


private:
    //==============================================================================
    // Your private member variables go here...



    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton cueButtons[6];
    juce::TextButton loadButton;


    juce::Slider lowFreqSlider;
    juce::Slider midFreqSlider;
    juce::Slider highFreqSlider;
    juce::Slider grainSlider;
    juce::Slider filterSlider;
    juce::Slider volumeSlider;
    juce::Slider dial1;

    juce::Random random;
    bool playing = false;

    double gain = 0.5;
    float phase = 0.f;
    float freq = 0.f;

    juce::AudioFormatManager formatManager;

    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;;

    juce::AudioTransportSource transportSource;

    bool loadURL(const juce::URL& audioURL);

    enum SoundType {Noise, Siren};

    SoundType currentSound = SoundType::Noise;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
