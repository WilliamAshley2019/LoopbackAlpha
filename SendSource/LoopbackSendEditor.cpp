#include "LoopbackSendEditor.h"

LoopbackSendAudioProcessorEditor::LoopbackSendAudioProcessorEditor(LoopbackSendAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(320, 160);
}

LoopbackSendAudioProcessorEditor::~LoopbackSendAudioProcessorEditor() = default;

void LoopbackSendAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("Loopback Send\n\nPlace on any mixer track\nyou want to route elsewhere",
        getLocalBounds().reduced(20), juce::Justification::centred, 4);
}

void LoopbackSendAudioProcessorEditor::resized()
{
}