/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MyFirstJUCEPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MyFirstJUCEPluginAudioProcessor();
    ~MyFirstJUCEPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    float noteOnVel;
    int bitcrusherRate;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo&);
    void pushNextSampleIntoFifo(float) noexcept;

    static constexpr auto fftOrder = 10;                // [1]
    static constexpr auto fftSize = 1 << fftOrder;     // [2]
    bool nextFFTBlockReady = false;                     // [7]
    std::array<float, fftSize> fifo;                    // [4]
    std::array<float, fftSize * 2> fftData;             // [5]


private:

    int fifoIndex = 0;                                  // [6]
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyFirstJUCEPluginAudioProcessor)
};
