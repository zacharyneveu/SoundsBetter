/*
  ==============================================================================

    SBLookAndFeel.h
    Created: 24 Jul 2020 9:50:44pm
    Author:  Zach Neveu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SBLookAndFeel  : public juce::LookAndFeel_V4, public juce::DeletedAtShutdown
{
public:
	static const juce::Colour SBOrange, SBYellow, SBOlive, SBBlue, SBForest;
	SBLookAndFeel();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SBLookAndFeel)
};
