/*
  ==============================================================================

    HeadphoneConv.cpp
    Created: 25 Jul 2020 12:58:19pm
    Author:  Zach Neveu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HeadphoneConv.h"
#include "AppConfig.hpp"

//==============================================================================
HeadphoneConv::HeadphoneConv() :
    selector("Headphone Selector")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    populateHeadphoneModels();
    selector.addItemList(getHeadphoneNames(), 1);
    selector.setSelectedId(1);
    selector.addListener(this);
    addAndMakeVisible(selector);

}

HeadphoneConv::~HeadphoneConv()
{
}

void HeadphoneConv::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("HeadphoneConv", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void HeadphoneConv::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> bounds = getBounds();
    selector.setBounds(bounds.removeFromTop(40));
}

void HeadphoneConv::populateHeadphoneModels()
{
    hp_names["None"] = "None";
    hp_names["HiFiMan"] = "HiFiMan_He4xx";
    hp_names["Bose"] = "Bose/HpCF_Bose_Noise_Cancelling_Headphones-700.wav";
    hp_names["Sennheiser"] = "Sennheiser_HD800";
}

StringArray HeadphoneConv::getHeadphoneNames()
{
    StringArray names;
    for(auto kv_pair : hp_names)
        names.add(kv_pair.first);
    return names;
}

void HeadphoneConv::comboBoxChanged(ComboBox *sel)
{
    String hp = sel->getText();
    String model = hp_names[hp];
    Logger::outputDebugString("Selected headphone: "+hp);
    AppConfig::Instance().downloadHPCF(model.toStdString());
}
