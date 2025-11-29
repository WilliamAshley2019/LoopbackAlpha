#include "LoopbackReturnProcessor.h"
#include "LoopbackReturnEditor.h"

LoopbackReturnAudioProcessor::LoopbackReturnAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    setLatencySamples(0);
}

LoopbackReturnAudioProcessor::~LoopbackReturnAudioProcessor() = default;

void LoopbackReturnAudioProcessor::prepareToPlay(double, int) {}

void LoopbackReturnAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer&)
{
    buffer.clear();

    const int numSamples = buffer.getNumSamples();

    // ✅ Read from Windows shared memory
    LoopbackShared::getInstance().read(
        buffer.getWritePointer(0),
        buffer.getNumChannels() >= 2 ? buffer.getWritePointer(1) : nullptr,
        numSamples
    );

    // Tiny DC offset to prevent Smart Disable
    if (numSamples > 0)
    {
        buffer.setSample(0, 0, buffer.getSample(0, 0) + 0.0000001f);
        if (buffer.getNumChannels() >= 2)
            buffer.setSample(1, 0, buffer.getSample(1, 0) + 0.0000001f);
    }
}

bool LoopbackReturnAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& out = layouts.getMainOutputChannelSet();
    return out == juce::AudioChannelSet::mono()
        || out == juce::AudioChannelSet::stereo();
}

juce::AudioProcessorEditor* LoopbackReturnAudioProcessor::createEditor()
{
    return new LoopbackReturnAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LoopbackReturnAudioProcessor();
}