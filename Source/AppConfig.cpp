#include "AppConfig.hpp"

#include <JuceHeader.h>
using namespace juce;


#define HPCF_BASE_URL "https://raw.githubusercontent.com/ShanonPearce/ASH-IR-Dataset/master/HpCFs/"

AppConfig::AppConfig() : app_data_dir (File::getSpecialLocation (File::userApplicationDataDirectory).getChildFile (JUCEApplication::getInstance()->getApplicationName())), hpcf_dir(app_data_dir.getChildFile("hpcfs"))
{
    if (! app_data_dir.exists())
        app_data_dir.createDirectory();

	if (! hpcf_dir.exists())
		hpcf_dir.createDirectory();
}

const File AppConfig::downloadHPCF(std::string file_name) const
{
    File hpcf = hpcf_dir.getChildFile(file_name);
    if(hpcf.exists())
    {
        hpcf.deleteFile();
    }
    
   
    /*
    AlertWindow win("Downloading Headphone Response...", "This shouldn't take long.", AlertWindow::NoIcon);
    win.addButton("OK", 1, KeyPress(13));
     */

    URL url(HPCF_BASE_URL+file_name);
    std::unique_ptr<URL::DownloadTask> dl_task = url.downloadToFile(hpcf, "", (URL::DownloadTask::Listener*)this);
    
    Logger::outputDebugString("Downloading file: "+url.toString(false));
    
    //win.runModalLoop();
    return hpcf;
}

void AppConfig::finished(URL::DownloadTask* task, bool success)
{
    dl_progress = 1.0;
    dl_complete = true;
    if(task->statusCode() != 200)
        Logger::outputDebugString("Download failed with code: "+String(task->statusCode()));
    else
        Logger::outputDebugString("Download Complete!");
}

void AppConfig::progress(URL::DownloadTask* task, int64_t downloaded, int64_t totalLength)
{
    dl_progress = (double)downloaded / (double)totalLength;
    Logger::outputDebugString("Downloading progress: "+String(dl_progress));
}
