#include "LoopbackReturnEditor.h"

LoopbackReturnAudioProcessorEditor::LoopbackReturnAudioProcessorEditor(LoopbackReturnAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(320, 180);
}

LoopbackReturnAudioProcessorEditor::~LoopbackReturnAudioProcessorEditor() = default;

void LoopbackReturnAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("Loopback Return\n\n"
        "Load as Instrument on a new mixer track\n"
        "Receives audio from Loopback Send",
        getLocalBounds().reduced(20),
        juce::Justification::centred, 5);
}

void LoopbackReturnAudioProcessorEditor::resized()
{
}