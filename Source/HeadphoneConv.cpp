/*
 ==============================================================================
 
 HeadphoneConv.cpp
 Created: 25 Jul 2020 12:58:19pm
 Author:  Zach Neveu
 
 ==============================================================================
 */

#include "HeadphoneConv.hpp"
#include "AppConfig.hpp"
#include "external/csv.hpp"
#include <JuceHeader.h>

using namespace juce;

//==============================================================================
HeadphoneConv::HeadphoneConv()
: selector("Headphone Selector"), waveform(1, waveform_fmt_mgr, waveform_cache), waveform_cache(1)
{
    waveform_fmt_mgr.registerBasicFormats();
    waveform.addChangeListener(this);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    populateHeadphoneModels();
    selector.addItemList(getHeadphoneNames(), 1);
    selector.addListener(this);
    selector.setSelectedId(1);
    addAndMakeVisible(selector);
}

HeadphoneConv::~HeadphoneConv() {}

void HeadphoneConv::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(
                                          juce::ResizableWindow::backgroundColourId)); // clear the background
    
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter());
    g.drawRoundedRectangle(waveform_bounds.toFloat(), 5.0, 1.0);
    
    g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));
    waveform.drawChannel(g, waveform_bounds, 0, waveform.getTotalLength(), 0, 1);
}

void HeadphoneConv::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> bounds = getBounds();
    const int waveform_padding = 10;
    bounds.removeFromTop(waveform_padding);
    bounds.removeFromRight(waveform_padding);
    bounds.removeFromLeft(waveform_padding);
    
    selector.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(waveform_padding);
    bounds.removeFromBottom(waveform_padding);
    
    waveform_bounds = bounds;
}

void HeadphoneConv::populateHeadphoneModels()
{
    File inf = File::getSpecialLocation(File::userHomeDirectory).getChildFile("ir_links.csv");
    io::CSVReader<3> irs(inf.getFullPathName().toStdString());
    irs.read_header(io::ignore_extra_column, "", "link", "stub");
    std::string link, stub;
    int idx;
    while(irs.read_row(idx, link, stub))
    {
        String stb(stub);
        String model = stb.fromLastOccurrenceOf("/", false, false);
        hp_names[model] = model+".wav";
        DBG("Loaded HPCF: "+model);
    }
}

StringArray HeadphoneConv::getHeadphoneNames()
{
    StringArray names;
    for (auto kv_pair : hp_names)
        names.add(kv_pair.first);
    return names;
}

void HeadphoneConv::comboBoxChanged(ComboBox* sel)
{
    String hp = sel->getText();
    String model = hp_names[hp];
    Logger::outputDebugString("Selected headphone: " + hp);
    ir_file = downloadHPCF(model.toStdString());
    waveform.setSource(new FileInputSource(ir_file));
}

File HeadphoneConv::downloadHPCF(std::string file_name)
{
    File hpcf_dir = AppConfig::Instance().getHPCFDir();
    
    File maker_dir = hpcf_dir;
    if(String(file_name).contains("/"))
        maker_dir = maker_dir.getChildFile(file_name.substr(0, file_name.find('/')));
    maker_dir.createDirectory();
    
    File hpcf = hpcf_dir.getChildFile(file_name);
    if (!hpcf.exists())
    {
        dl_progress = 0.0;
        AlertWindow win(
                        "Downloading Headphone Response...", "This shouldn't take long.", AlertWindow::NoIcon);
        win.addButton("OK", 1, KeyPress(13));
        win.addProgressBarComponent(dl_progress);
        
        URL url(AppConfig::Instance().getHPCFURL() + file_name);
        std::unique_ptr<URL::DownloadTask> dl_task =
        url.downloadToFile(hpcf, "", (URL::DownloadTask::Listener*) this);
        Logger::outputDebugString("Downloading file: " + url.toString(false));
        Logger::outputDebugString("Saving to: " + hpcf.getFullPathName());
        
        win.runModalLoop();
        while(!dl_task->isFinished())
        {
            Thread::sleep(10);
            DBG("sleeping");
        }
    }
    return hpcf;
}

void HeadphoneConv::finished(URL::DownloadTask* task, bool success)
{
    dl_progress = 1.0;
    if (task->statusCode() != 200)
        Logger::outputDebugString("Download failed with code: " + String(task->statusCode()));
    else
    {
        Logger::outputDebugString("Download Complete!");
    }
}

void HeadphoneConv::progress(URL::DownloadTask* task, int64_t downloaded, int64_t totalLength)
{
    dl_progress = (double) (downloaded / totalLength);
    Logger::outputDebugString("Downloading progress: " + String(dl_progress));
}

// waveform will call this when it has loaded
void HeadphoneConv::changeListenerCallback(ChangeBroadcaster* source) {
    repaint();
}
