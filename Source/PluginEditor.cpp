/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyFirstJUCEPluginAudioProcessorEditor::MyFirstJUCEPluginAudioProcessorEditor (MyFirstJUCEPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0,1.0,0.05);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);

    midiVolume.addListener(this);

    // this function adds the slider to the editor
    addAndMakeVisible(&midiVolume);
}

MyFirstJUCEPluginAudioProcessorEditor::~MyFirstJUCEPluginAudioProcessorEditor()
{
}

//==============================================================================
void MyFirstJUCEPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Volume", getLocalBounds(), juce::Justification::centred, 1);
}

void MyFirstJUCEPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    

    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

void MyFirstJUCEPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}
