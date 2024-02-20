#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }


    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setButtonText("PLAY");

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    stopButton.setButtonText("STOP");

    for (int i = 0; i < 6; i++) {
        cueButtons[i].addListener(this);
        addAndMakeVisible(cueButtons[i]);
    }

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setButtonText("LOAD");

    addAndMakeVisible(lowFreqSlider);
    lowFreqSlider.addListener(this);
    lowFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lowFreqSlider.setNumDecimalPlacesToDisplay(0);

    addAndMakeVisible(midFreqSlider);
    midFreqSlider.addListener(this);
    midFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    midFreqSlider.setNumDecimalPlacesToDisplay(0);

    addAndMakeVisible(highFreqSlider);
    highFreqSlider.addListener(this);
    highFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    highFreqSlider.setNumDecimalPlacesToDisplay(0);

    addAndMakeVisible(grainSlider);
    grainSlider.addListener(this);
    grainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    grainSlider.setNumDecimalPlacesToDisplay(0);

    addAndMakeVisible(filterSlider);
    filterSlider.addListener(this);
    filterSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    filterSlider.setNumDecimalPlacesToDisplay(0);

    addAndMakeVisible(volumeSlider);
    volumeSlider.addListener(this);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    volumeSlider.setNumDecimalPlacesToDisplay(0);
    volumeSlider.setRange(0, 1);
    volumeSlider.setValue(0.5);


    addAndMakeVisible(dial1);
    dial1.addListener(this);
    dial1.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dial1.setNumDecimalPlacesToDisplay(0);

    formatManager.registerBasicFormats();

    juce::URL audioURL("file:///C:/Users/kivir/Documents/UNI/CC/YEAR%202/EXTENDENED%20C++/juce-7.0.9-windows/JUCE/apps/DJ_application/Assets/OneLoveByShubh.wav");


    juce::AudioFormatReader* reader = formatManager.createReaderFor(audioURL.getLocalFile());

    if (reader) {
        auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource = std::move(newSource);
    }
    else {
        DBG(" Something went wrong loading the file ");
    }

}


MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}


void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (!playing) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);

    //int numChannels = block.buffer->getNumChannels();
    //auto* leftChannel = numChannels > 0 ? block.buffer->getWritePointer(0,
    //    block.startSample) : nullptr;
    //auto* rightChannel = numChannels > 1 ? block.buffer->getWritePointer(1,
    //    block.startSample) : nullptr;

    //for (int i = 0; i < block.numSamples; ++i) {
    //    auto sample = std::fmod(phase, 1.f) * 2. - 1.;
    //    sample *= 0.1 * gain;
    //    phase += std::fmod(freq, 0.01f);
    //    freq += 0.0000005f;
    //    if (leftChannel) leftChannel[i] = sample;
    //    if (rightChannel) rightChannel[i] = sample;
    //}

}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!

}

bool MainComponent::loadURL(const juce::URL& audioURL) {
    auto* reader = formatManager.createReaderFor(audioURL.getLocalFile());
    if (reader) {
        std::unique_ptr < juce::AudioFormatReaderSource > newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        return true;
    }
    return false;
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    // Calculate the button size and position
    int buttonWidth = 100;
    int buttonHeight = 30;
    int xPosition = 10; 
    int yPosition = getHeight() - 2 * buttonHeight - 20; 

    int sliderWidth = 150;
    int sliderHeight = 80;
    int sliderXpos = getWidth() - sliderWidth - 10;
    int sliderYpos = 110;

    int cueXpos = 10;
    int cueYpos = 30;

    for (int i = 0; i < 6; i++) {

        if (i % 2 == 0) { // Even, new column
            cueXpos = 10;
            cueYpos += 110;
        }
        else { // Odd, increment x
            cueXpos += 110;
        }
        cueButtons[i].setBounds(cueXpos, cueYpos, 80, 80);
    }

    // Set the bounds for the play button
    playButton.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);

    // Set the bounds for the stop button
    stopButton.setBounds(xPosition, yPosition + buttonHeight + 10, buttonWidth, buttonHeight);

    loadButton.setBounds(xPosition, yPosition + buttonHeight - 70, buttonWidth, buttonHeight);

    // Set the bounds for the lowFreq slider
    lowFreqSlider.setBounds(sliderXpos, sliderYpos, sliderWidth, sliderHeight);
    sliderYpos += 70;

    // Set the bounds for the midFreq slider
    midFreqSlider.setBounds(sliderXpos, sliderYpos, sliderWidth, sliderHeight);
    sliderYpos += 70;

    // set the bounds for the highFreq slider
    highFreqSlider.setBounds(sliderXpos, sliderYpos, sliderWidth, sliderHeight);
    sliderYpos += 70;

    // set the bounds for the garin slider
    grainSlider.setBounds(sliderXpos, sliderYpos, sliderWidth, sliderHeight);
    sliderYpos += 70;

    // set the bounds for the filter slider
    filterSlider.setBounds(sliderXpos, sliderYpos, sliderWidth, sliderHeight);
    sliderYpos += 70;
    
    // set the bounds for the volume slider
    volumeSlider.setBounds(sliderXpos - 50, 140, 35, 300);

    // set the bounds for the dail
    dial1.setBounds(sliderXpos - 480, 110, 400, 400);

   


    DBG("w:" << getWidth() << " h:" << getHeight());

}

void MainComponent::buttonClicked(juce::Button* button) {
    DBG(" MainComponent :: buttonClicked : Button clicked ");
    if (&playButton == button) { // clicked button has same memory address as playButton
        DBG(" MainComponent :: buttonClicked : playButton ");
        playing = true;
        freq = 0;
        transportSource.setPosition(0);
        transportSource.start();
    }
    else if (&stopButton == button) {
        DBG(" MainComponent :: buttonClicked : stopButton ");
        playing = false;
        transportSource.stop();
    }
    if (&cueButtons[6] == button) {
        DBG(" MainComponent :: buttonClicked : cueButtons ");

    }
    if (button == &cueButtons[0]) {
        currentSound = SoundType::Noise;

    }
    else if (button == &cueButtons[1]) {
        currentSound = SoundType::Siren;
    }

    for (int i = 0; i < 6; ++i) {
        if (button == &cueButtons[i]) {
            DBG(" MainComponent :: buttonClicked : cueButtons ");
            if (i == 0) {
                currentSound = SoundType::Noise;
            }
            else if (i == 1) {
                currentSound = SoundType::Siren;
            }
        }
    }
    if (&loadButton == button) {
        juce::FileChooser chooser{ " Select a sound file ... " };
        if (chooser.browseForFileToOpen()) {
            loadURL(juce::URL{chooser.getResult()});
        }
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (&lowFreqSlider == slider) {
        DBG(
            " MainComponent :: sliderValueChanged : lowFreqSlider " << lowFreqSlider.getValue()
        );
    }
    else if (&midFreqSlider == slider) {
        DBG(
            " MainComponent :: sliderValueChanged : midFreqSlider " << midFreqSlider.getValue()
        );
    }
    else if (&highFreqSlider == slider) {
        DBG(
            " MainComponent :: sliderValueChanged : highFreqSlider " << highFreqSlider.getValue()
        );
    }
    else if (&grainSlider == slider) {
        DBG(
            " MainComponent :: sliderValueChanged : grainSlider " << grainSlider.getValue()
        );
    }
    else if (&filterSlider == slider) {
        DBG(
            " MainComponent :: sliderValueChanged : filterSlider " << filterSlider.getValue()
        );
    }
    else if (&volumeSlider == slider) {
        DBG(
            " MainComponent :: sliderValueChanged : volumeSlider " << volumeSlider.getValue()
        );
        gain = volumeSlider.getValue();
        transportSource.setGain(gain);
    }
    else if (&dial1 == slider) {
        DBG(
            " MainComponent :: sliderValueChanged : dial1Slider " << dial1.getValue()
        );
    }
}

