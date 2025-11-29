#pragma once
#include <JuceHeader.h>
#include "LoopbackReturnProcessor.h"

class LoopbackReturnAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit LoopbackReturnAudioProcessorEditor(LoopbackReturnAudioProcessor&);
    ~LoopbackReturnAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    LoopbackReturnAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoopbackReturnAudioProcessorEditor)
};