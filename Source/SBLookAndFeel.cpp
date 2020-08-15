/*
  ==============================================================================

    SBLookAndFeel.cpp
    Created: 24 Jul 2020 9:50:44pm
    Author:  Zach Neveu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SBLookAndFeel.hpp"

using namespace juce;

/** Hex colors 
 * Orange: #fab763
 * Yellow: #ddc65d
 * Olive: #6d9161
 * Blue: #28686e
 * Forest: #1d4038
 */

const Colour SBLookAndFeel::SBOrange(Colour(250, 183, 99));
const Colour SBLookAndFeel::SBYellow(Colour(221, 198, 95));
const Colour SBLookAndFeel::SBOlive (Colour(109, 145, 97));
const Colour SBLookAndFeel::SBBlue  (Colour(40, 104, 110));
const Colour SBLookAndFeel::SBForest(Colour(29, 64, 56));

//==============================================================================
SBLookAndFeel::SBLookAndFeel()
{
    //setColour (ResizableWindow::backgroundColourId, SBForest);
    setColour (ColourScheme::UIColour::windowBackground, SBForest);
    setColour (ColourScheme::UIColour::widgetBackground, SBOlive);
    setColour (ColourScheme::UIColour::menuBackground, SBOlive);
    setColour (ColourScheme::UIColour::defaultFill, SBBlue);
    setColour (ComboBox::backgroundColourId, SBOlive);
    setColour (PopupMenu::backgroundColourId, SBOlive);
    setColour (TextButton::buttonColourId, SBForest);
    setColour (ToggleButton::tickDisabledColourId, SBYellow);
    setColour (ToggleButton::tickColourId, SBOrange);
    setColour (ListBox::backgroundColourId, SBForest);
    setColour (Slider::thumbColourId, SBOrange);
}
