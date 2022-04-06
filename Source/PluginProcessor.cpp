/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //mPreDelaySliderValue = 0.0f;
    mWetDrySliderValue = 0.0f;
    
    mPath = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFullPathName();
    
    mName = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFileName();
    
    mPath = mPath.upToLastOccurrenceOf(mName, false, false);
    
    mPresetFlag = 1;
    
    //do this for every IR file
        
    //mPreDelayRingBuf
    
    //preDelayBufLength = 1;
    //preDelayReadPosition = 0;
    //preDelayWritePosition = 0;
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
void NewProjectAudioProcessor::changeIR()
{
    //switch between different IR files in the Resources folder
    switch (mPresetFlag)
        {
        case LivingRoom:
                mImpulseResponse = mPath + "../Resources/Living_Room.wav";
            break;
            
        case ParkingGarage:
                mImpulseResponse = mPath + "../Resources/Parking_Garage.wav";
            break;
                
        case Stairwell:
                mImpulseResponse = mPath + "../Resources/Stairwell.wav";
            break;
                
        case WorkoutRoom:
                    mImpulseResponse = mPath + "../Resources/Workout_Room.wav";
                break;
                
        case Bathroom:
                    mImpulseResponse = mPath + "../Resources/Bathroom.wav";
                break;
                
        case Bedroom:
                    mImpulseResponse = mPath + "../Resources/Bedroom.wav";
                break;
                
        case LargeConcreteRoom:
                    mImpulseResponse = mPath + "../Resources/Large_Concrete_Room.wav";
                break;
                
        case Foyer:
                    mImpulseResponse = mPath + "../Resources/Foyer.wav";
                break;
                
        case YogaStudio:
                    mImpulseResponse = mPath + "../Resources/Yoga_Studio.wav";
                break;
                
        case none:
            break;
            
        default:
            break;
        }
    convolution.loadImpulseResponse(mImpulseResponse, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
}

const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}




bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mBlockSize = samplesPerBlock;
    mSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec processSpec;
    processSpec.sampleRate = mSampleRate;
    processSpec.maximumBlockSize = mBlockSize;
    processSpec.numChannels = getTotalNumInputChannels();
    
    convolution.prepare(processSpec);
    
    /*preDelayBufLength  = (int) 2.0 * sampleRate;
        if (preDelayBufLength < 1)
            preDelayBufLength = 1;
    
    preDelayReadPosition = (int) (preDelayWritePosition - (mPreDelaySliderValue * getSampleRate()) + preDelayBufLength) % preDelayBufLength;*/
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    //const int numSamples = buffer.getNumSamples();
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //make copy of the incoming buffer here
    juce::AudioBuffer<float> wetBuffer;
    wetBuffer.makeCopyOf(buffer);
    
//package the wet buffer object into an audio block object to prepare it for convolution
    
    juce::dsp::AudioBlock<float> convAudioBlock(wetBuffer);
    //process the wet buffer audio block to get the convolved/reverbed version of the buffer
    convolution.process( (juce::dsp::ProcessContextReplacing<float>) convAudioBlock);
    
    //change the gain of the dry buffer
    buffer.applyGain(1.0f - mWetDrySliderValue);
    //change the gain of the wet buffer
    wetBuffer.applyGain(mWetDrySliderValue);
    
    
    //pre-delay
    //learn how to use the abstract fifo class
    //or make your own pre-delay code
    //atec::RingBuffer, let's you write samples into it and read samples out of it delayed; .write and .read methods
    //Echo Delay plug-in
    //Don't need to do any interpolation, you can read uninterpolated samples a certain samples
    //declare RingBuffer in processor header, initialize in constructor
    //should be two lines, .write and then .read
    
    //this is where I'm trying to put my pre-delay code
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            /*int mPDReadPosition = preDelayReadPosition;
            int mPDWritePosition = preDelayWritePosition;*/
            
            /*for (int i = 0; i < numSamples; ++i)
            {
                    const float in = channelData[i];
                    float out = 0.0;
                
                    out = (buffer * in + wetBuffer * delayData[mPDReadPosition]);
                            
                    delayData[mPDWritePosition] = in + (delayData[mPDReadPosition]);
                        
                        if (++mPDReadPosition >= preDelayBufLength)
                            mPDReadPosition = 0;
                        if (++mPDWritePosition >= preDelayBufLength)
                            mPDWritePosition = 0;
                        
                        channelData[i] = out;
            }*/
        }
    
    //mPreDelayRingBuf.advanceWriteIdx(bufSize);
    
    //auto bufferSize = buffer.getNumSamples();
    //auto wetBufferSize = wetBuffer.getNumSamples();
    
    //make a mix of both the dry original buffer and the wet buffer
    for (auto channel = 0; channel < totalNumInputChannels; ++channel)
        buffer.addFrom(channel, 0, wetBuffer, channel, 0, wetBuffer.getNumSamples());
    
    //put dry/wet mix into buffer so it gets back out to the DAW
    
    
    
    //pre-delay would be delaying the wet signal
    //take a look at the delay plug-in from class
    //you would set the wet signal a certain number of samples back
    
   
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
