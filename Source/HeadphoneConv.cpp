/*
  ==============================================================================

    HeadphoneConv.cpp
    Created: 25 Jul 2020 12:58:19pm
    Author:  Zach Neveu

  ==============================================================================
*/

#include "HeadphoneConv.h"
#include "AppConfig.hpp"
#include <JuceHeader.h>

//==============================================================================
HeadphoneConv::HeadphoneConv() : selector ("Headphone Selector")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    populateHeadphoneModels();
    selector.addItemList (getHeadphoneNames(), 1);
    selector.addListener (this);
    selector.setSelectedId (1);
    addAndMakeVisible (selector);
}

HeadphoneConv::~HeadphoneConv() {}

void HeadphoneConv::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (
        juce::ResizableWindow::backgroundColourId)); // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1); // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("HeadphoneConv",
                getLocalBounds(),
                juce::Justification::centred,
                true); // draw some placeholder text
}

void HeadphoneConv::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> bounds = getBounds();
    selector.setBounds (bounds.removeFromTop (40));
}

void HeadphoneConv::populateHeadphoneModels()
{
    hp_names["None"] = "Other/None.wav";
    hp_names["HiFiMan"] = "HiFiMAN/HpCF_Massdrop_HiFiMAN_He4XX.wav";
    hp_names["Bose"] = "Bose/HpCF_Bose_Noise_Cancelling_Headphones-700.wav";
    hp_names["Sennheiser"] = "Sennheiser/HpCF_Sennheiser_HD800S_A.wav";
}

StringArray HeadphoneConv::getHeadphoneNames()
{
    StringArray names;
    for (auto kv_pair : hp_names)
        names.add (kv_pair.first);
    return names;
}

void HeadphoneConv::comboBoxChanged (ComboBox* sel)
{
    String hp = sel->getText();
    String model = hp_names[hp];
    Logger::outputDebugString ("Selected headphone: " + hp);
    ir_file = downloadHPCF (model.toStdString());
}

File HeadphoneConv::downloadHPCF (std::string file_name)
{
    File hpcf_dir = AppConfig::Instance().getHPCFDir();

    File maker_dir = hpcf_dir.getChildFile (file_name.substr (0, file_name.find ('/')));
    maker_dir.createDirectory();

    File hpcf = hpcf_dir.getChildFile (file_name);
    if (! hpcf.exists())
    {
        dl_progress = 0.0;
        AlertWindow win (
            "Downloading Headphone Response...", "This shouldn't take long.", AlertWindow::NoIcon);
        win.addButton ("OK", 1, KeyPress (13));
        win.addProgressBarComponent (dl_progress);

        URL url (AppConfig::Instance().getHPCFURL() + file_name);
        std::unique_ptr<URL::DownloadTask> dl_task =
            url.downloadToFile (hpcf, "", (URL::DownloadTask::Listener*) this);
        Logger::outputDebugString ("Downloading file: " + url.toString (false));
        Logger::outputDebugString ("Saving to: " + hpcf.getFullPathName());

        win.runModalLoop();
    }
    return hpcf;
}

void HeadphoneConv::finished (URL::DownloadTask* task, bool success)
{
    dl_progress = 1.0;
    if (task->statusCode() != 200)
        Logger::outputDebugString ("Download failed with code: " + String (task->statusCode()));
    else
        Logger::outputDebugString ("Download Complete!");
}

void HeadphoneConv::progress (URL::DownloadTask* task, int64_t downloaded, int64_t totalLength)
{
    dl_progress = (double) (downloaded / totalLength);
    Logger::outputDebugString ("Downloading progress: " + String (dl_progress));
}
