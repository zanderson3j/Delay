/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    delaySlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::NoTextBox);
    delayLabel = std::make_unique<Label>("", "Delay");
    addAndMakeVisible(delaySlider.get());
    delayLabel->attachToComponent(delaySlider.get(), false);
    delayLabel->setJustificationType(Justification::centred);
    delayAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                        "DLY",
                                                                                        *delaySlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::findColourForName("deeppink", getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)));
}

void DelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    bounds.reduce(40,40);
    
    Grid grid;
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    
    // 4 evenly spaced columns (25% of available space on x-axis)
    grid.templateColumns = { Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1))
    };
    
    // 2 evenly spaced rows (50% of available space on y-axis)
    grid.templateRows = { Track (Fr (1)), Track (Fr (1)) };
    
    // margin between each space
    grid.rowGap = Grid::Px(10);
    grid.columnGap = Grid::Px(10);
    
    grid.items.add(delaySlider.get());
    
    grid.performLayout(bounds);
}
