/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
   /* mPreDelayTimeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mPreDelayTimeSlider.setRange(0.000f, 1.000f, 0.005f);
    mPreDelayTimeSlider.setValue(0);
    addAndMakeVisible(&mPreDelayTimeSlider);
    mPreDelayTimeSlider.addListener(this);
    
    addAndMakeVisible(&mPreDelayTimeLabel);
    mPreDelayTimeLabel.setText("Pre-Delay", juce::dontSendNotification);
    mPreDelayTimeLabel.attachToComponent(&mPreDelayTimeSlider, true);
    mPreDelayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    mPreDelayTimeLabel.setJustificationType(juce::Justification::centredTop);*/
    
    mWetDrySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mWetDrySlider.setRange(0.000f, 1.000f, 0.005f);
    mWetDrySlider.setValue(0);
    addAndMakeVisible(&mWetDrySlider);
    mWetDrySlider.addListener(this);
    
    addAndMakeVisible(&mWetDryLabel);
    mWetDryLabel.setText("Wet/Dry", juce::dontSendNotification);
    mWetDryLabel.attachToComponent(&mWetDrySlider, true);
    mWetDryLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    mWetDryLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    //mOutputGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    //mOutputGainSlider.setRange(0.0f, 20.0f, 0.005f);
    //mOutputGainSlider.setValue(0);
    //addAndMakeVisible(&mOutputGainSlider);
    //mOutputGainSlider.addListener(this);
    
    //addAndMakeVisible(&mOutputGainLabel);
    //mOutputGainLabel.setText("Output", juce::dontSendNotification);
    //mOutputGainLabel.attachToComponent(&mOutputGainLabel, true);
    //mOutputGainLabel.setColour(juce::Label::textColourId, juce::Colours::magenta);
    //mOutputGainLabel.setJustificationType(juce::Justification::centredTop);
    
    mReverbLocationBox.addItem("Bathroom", 1);
    mReverbLocationBox.addItem("Bedroom", 2);
    mReverbLocationBox.addItem("Foyer", 3);
    mReverbLocationBox.addItem("Large Concrete Room", 4);
    mReverbLocationBox.addItem("Living Room", 5);
    mReverbLocationBox.addItem("Parking Garage", 6);
    mReverbLocationBox.addItem("Stairwell", 7);
    mReverbLocationBox.addItem("Workout Room", 8);
    mReverbLocationBox.addItem("Yoga Studio", 9);
    mReverbLocationBox.setSelectedId(1);
    addAndMakeVisible(&mReverbLocationBox);
    mReverbLocationBox.addListener(this);
    
    addAndMakeVisible(&mReverbLocationLabel);
    mReverbLocationLabel.setText("Location", juce::dontSendNotification);
    mReverbLocationLabel.attachToComponent(&mReverbLocationBox, true);
    mReverbLocationLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    mReverbLocationLabel.setJustificationType(juce::Justification::horizontallyCentred);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    mPreDelayTimeSlider.removeListener(this);
    mWetDrySlider.removeListener(this);
    mReverbLocationBox.removeListener(this);
}
    
void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &mWetDrySlider)
    {
        audioProcessor.mWetDrySliderValue = mWetDrySlider.getValue();
    }
        //audioProcessor.mPreDelaySliderValue = mPreDelayTimeSlider.getValue();
}

void NewProjectAudioProcessorEditor::comboBoxChanged(juce::ComboBox *combobox)
{
    if (combobox == &mReverbLocationBox)
    {
        audioProcessor.mPresetFlag = mReverbLocationBox.getSelectedId();
        audioProcessor.changeIR();
    }
}
//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::darkcyan);

    g.setColour (juce::Colours::beige);
    g.setFont (45.0f);
    g.drawFittedText ("Convolution Reverb", getLocalBounds(), juce::Justification::centredTop, 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    //mPreDelayTimeSlider.setBounds(100, 220, 120, 120);
    mWetDrySlider.setBounds(150, 220, 350, 50);
    //mOutputGainSlider.setBounds(150, 320, 300, 50);
    mReverbLocationBox.setBounds(225, 100, 200, 50);
}
