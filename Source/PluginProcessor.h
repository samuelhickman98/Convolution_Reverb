/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum verbType
{
    Bathroom = 1,
    Bedroom,
    Foyer,
    LargeConcreteRoom,
    LivingRoom,
    ParkingGarage,
    Stairwell,
    WorkoutRoom,
    YogaStudio,
    none
};

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

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
    double mPreDelaySliderValue;
    double mWetDrySliderValue;
    double mOutputSliderValue;
    
    int mPreDelayTimeSamps;
    int mPresetFlag;
    
    void changeIR();
    void selectFile();
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
    
    //void clearAllBuffers(juce::AudioBuffer<float>& buffer);
    
    int ringBufSize;
    //void fillBuffer (int channel, int bufferSize, int wetBufferSize, float* channelData);
    
    double mSampleRate;
    double mBlockSize;
    double mNumChannels;
    
    //int preDelayBufLength;
    //int preDelayReadPosition;
    //int preDelayWritePosition;
    
    juce::File mImpulseResponse;
    juce::String mPath;
    juce::String mName;
    
    juce::dsp::Convolution convolution;
    juce::AudioBuffer<float> mPreDelayRingBuf;
    
    std::unique_ptr<juce::FileChooser> mFileChooserPtr;
    bool loadChosenFile(juce::File file);
};
