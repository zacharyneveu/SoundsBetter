/*
  ==============================================================================

    HeadphoneConv.h
    Created: 25 Jul 2020 12:58:19pm
    Author:  Zach Neveu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

//==============================================================================
/*
*/
class HeadphoneConv  : public Component,
                       public ComboBox::Listener
{
public:
    HeadphoneConv();
    ~HeadphoneConv() override;
    
    /**
     * @brief Gets headphone models from database and stores them in map
     */
    void populateHeadphoneModels();
    /** @brief Gets list of headphone names from map */
    StringArray getHeadphoneNames();
    
    void comboBoxChanged(ComboBox* sel) override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    dsp::Convolution conv;
    std::map<String, String> hp_names;
    ComboBox selector;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeadphoneConv)
};
