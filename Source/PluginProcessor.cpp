/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
_3bandEqAudioProcessor::_3bandEqAudioProcessor()
{
}

_3bandEqAudioProcessor::~_3bandEqAudioProcessor()
{
}

//==============================================================================
const String _3bandEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _3bandEqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _3bandEqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double _3bandEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _3bandEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _3bandEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _3bandEqAudioProcessor::setCurrentProgram (int index)
{
}

const String _3bandEqAudioProcessor::getProgramName (int index)
{
    return String();
}

void _3bandEqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void _3bandEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	SR = getSampleRate();
    
    toneL.initialiseWave(500, 0.25, SR, ToneGenerator::NOISE);
    toneR.initialiseWave(500, 0.25, SR, ToneGenerator::NOISE);
    
    freq = 10000;
    gain = 5;
    q = 10;
    control = 0;
}

void _3bandEqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _3bandEqAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void _3bandEqAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        
        float output;
        float input;
        
        BiquadFilter::filterType type = static_cast<BiquadFilter::filterType>(control);
        
        filterL.setParameters(freq, gain, q, type);
		filterR.setParameters(freq, gain, q, type);
        
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            if (channel == 0)
            {
                input = toneL.getValue();
                filterL.addSample(input);
                output = filterL.getSample();                           
            }
			else
			{
				input = toneR.getValue();
				filterR.addSample(input);
				output = filterR.getSample();
			}

			channelData[i] = output;
        }

        // ..do something to the data...
    }
}

//==============================================================================
bool _3bandEqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* _3bandEqAudioProcessor::createEditor()
{
    return new _3bandEqAudioProcessorEditor (*this);
}

//==============================================================================
void _3bandEqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _3bandEqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _3bandEqAudioProcessor();
}
