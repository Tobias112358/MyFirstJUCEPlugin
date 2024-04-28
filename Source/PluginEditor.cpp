/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::Image spectrogramImage;


//==============================================================================
MyFirstJUCEPluginAudioProcessorEditor::MyFirstJUCEPluginAudioProcessorEditor (MyFirstJUCEPluginAudioProcessor& p)
    : forwardFFT(MyFirstJUCEPluginAudioProcessor::fftOrder), AudioProcessorEditor (&p), audioProcessor (p), spectrogramImage(juce::Image::RGB, 400, 150, true)
{

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.        
    setOpaque (true);

    startTimerHz(60);
    setSize (400, 300);

    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0,1.0,0.05);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);

    bitcrusher.setName("Rate");
    bitcrusher.setRange(1, 50, 1); // division rate (rate / x)
    bitcrusher.setSliderStyle(juce::Slider::LinearBarVertical);
    bitcrusher.setPopupDisplayEnabled(true, false, this);
    bitcrusher.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    bitcrusher.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    bitcrusher.setTextValueSuffix(" Rate");
    bitcrusher.setTextBoxStyle(juce::Slider::TextBoxBelow, true,200, 20);
    bitcrusher.setValue(1);

    midiVolume.addListener(this);
    bitcrusher.addListener(this);

    // this function adds the slider to the editor
    addAndMakeVisible(&midiVolume);
    addAndMakeVisible(&bitcrusher);
}

MyFirstJUCEPluginAudioProcessorEditor::~MyFirstJUCEPluginAudioProcessorEditor()
{
}

//==============================================================================
void MyFirstJUCEPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setOpacity(1.0f);
    g.drawImage(spectrogramImage, getLocalBounds().toFloat());

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("", getLocalBounds(), juce::Justification::centred, 1);
}

void MyFirstJUCEPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    

    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
    bitcrusher.setBounds(340, 30, 20, getHeight() - 60);
}

void MyFirstJUCEPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();

    audioProcessor.bitcrusherRate = bitcrusher.getValue();
}

void MyFirstJUCEPluginAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.nextFFTBlockReady)
    {
        drawNextLineOfSpectrogram();
        audioProcessor.nextFFTBlockReady = false;
        repaint();
    }
}

void MyFirstJUCEPluginAudioProcessorEditor::drawNextLineOfSpectrogram()
{
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight = spectrogramImage.getHeight();

    // first, shuffle our image leftwards by 1 pixel..
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);         // [1]

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(audioProcessor.fftData.data());                   // [2]

    // find the range of values produced, so we can scale our rendering to
    // show up the detail clearly
    auto maxLevel = juce::FloatVectorOperations::findMinAndMax(audioProcessor.fftData.data(), audioProcessor.fftSize / 2); // [3]

    for (auto y = 1; y < imageHeight; ++y)                                              // [4]
    {
        auto skewedProportionY = 1.0f - std::exp(std::log((float)y / (float)imageHeight) * 0.2f);
        auto fftDataIndex = (size_t)juce::jlimit(0, audioProcessor.fftSize / 2, (int)(skewedProportionY * audioProcessor.fftSize / 2));
        auto level = juce::jmap(audioProcessor.fftData[fftDataIndex], 0.0f, juce::jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

        spectrogramImage.setPixelAt(rightHandEdge, y, juce::Colour::fromHSV(level, 1.0f, level, 1.0f)); // [5]
    }
}
