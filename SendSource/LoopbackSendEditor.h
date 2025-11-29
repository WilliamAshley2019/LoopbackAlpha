#pragma once
#include <JuceHeader.h>
#include "LoopbackSendProcessor.h"    // ← THIS WAS THE BUG! Now fixed

class LoopbackSendAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit LoopbackSendAudioProcessorEditor(LoopbackSendAudioProcessor&);
    ~LoopbackSendAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    LoopbackSendAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoopbackSendAudioProcessorEditor)
};