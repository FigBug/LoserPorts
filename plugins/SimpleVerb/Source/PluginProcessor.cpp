#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

static juce::String percentTextFunction (const gin::Parameter&, float v)
{
    return juce::String (juce::roundToInt (v * 100));
}

//==============================================================================
AudioProcessor::AudioProcessor()
{
    room        = addExtParam ("room",      "Room",     "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.3f, 0.0f, percentTextFunction);
    damp        = addExtParam ("damp",      "Damp",     "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.6f, 0.1f, percentTextFunction);
    preDelay    = addExtParam ("predelay",  "PreDelay", "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.3f, 0.0f, percentTextFunction);
    lp          = addExtParam ("lp",        "LP",       "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.9f, 0.1f, percentTextFunction);
    hp          = addExtParam ("hp",        "HP",       "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.1f, 0.1f, percentTextFunction);
    wet         = addExtParam ("wet",       "Wet",      "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.4f, 0.1f, percentTextFunction);
    dry         = addExtParam ("dry",       "Dry",      "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.3f, 0.1f, percentTextFunction);
}

AudioProcessor::~AudioProcessor()
{
}

//==============================================================================
void AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);
    
    reverb.setSampleRate (float (sampleRate));
}

void AudioProcessor::reset()
{
    gin::Processor::reset();
}

void AudioProcessor::releaseResources()
{
}

void AudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    int numSamples = buffer.getNumSamples();
    if (isSmoothing())
    {
        int pos = 0;
        
        while (pos < numSamples)
        {
            auto workBuffer = gin::sliceBuffer (buffer, pos, 1);
            
            reverb.setParameters (room->getProcValue (numSamples), damp->getProcValue (numSamples),
                                  preDelay->getProcValue (numSamples), lp->getProcValue (numSamples),
                                  hp->getProcValue (numSamples), wet->getProcValue (numSamples),
                                  dry->getProcValue (numSamples));

            reverb.process (workBuffer);
            pos++;
        }
    }
    else
    {
        reverb.setParameters (room->getProcValue (numSamples), damp->getProcValue (numSamples),
                              preDelay->getProcValue (numSamples), lp->getProcValue (numSamples),
                              hp->getProcValue (numSamples), wet->getProcValue (numSamples),
                              dry->getProcValue (numSamples));
        
        reverb.process (buffer);
    }
}

//==============================================================================
bool AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioProcessor::createEditor()
{
    return new AudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProcessor();
}
