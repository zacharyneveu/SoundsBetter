/*
  ==============================================================================

    SBLookAndFeel.cpp
    Created: 24 Jul 2020 9:50:44pm
    Author:  Zach Neveu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SBLookAndFeel.h"

using namespace juce;

/** Hex colors 
 * Orange: #fab763
 * Yellow: #ddc65d
 * Olive: #6d9161
 * Blue: #28686e
 * Forest: #1d4038
 */

const Colour SBLookAndFeel::SBOrange(Colour::fromString("#fab763"));
const Colour SBLookAndFeel::SBYellow(Colour::fromString("#ddc65d"));
const Colour SBLookAndFeel::SBOlive (Colour::fromString("#6d9161"));
const Colour SBLookAndFeel::SBBlue  (Colour::fromString("#28686e"));
const Colour SBLookAndFeel::SBForest(Colour::fromString("#1d4038"));

//==============================================================================
SBLookAndFeel::SBLookAndFeel()
{
    setColour (ResizableWindow::backgroundColourId, SBForest);
    setColour (ColourScheme::UIColour::windowBackground, SBForest);
    setColour (ColourScheme::UIColour::widgetBackground, SBForest);
    setColour (ColourScheme::UIColour::menuBackground, SBForest);
    setColour (ColourScheme::UIColour::defaultFill, SBForest);
    setColour (ComboBox::backgroundColourId, SBForest);
    setColour (PopupMenu::backgroundColourId, SBForest);
    setColour (TextButton::buttonColourId, SBForest);
    setColour (ToggleButton::tickDisabledColourId, SBYellow);
    setColour (ToggleButton::tickColourId, SBOrange);
    setColour (ListBox::backgroundColourId, SBForest);
    setColour (Slider::thumbColourId, SBOrange);
}
