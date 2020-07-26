#include "AppConfig.hpp"

#include <JuceHeader.h>
using namespace juce;

AppConfig::AppConfig()
    : app_data_dir (File::getSpecialLocation (File::userApplicationDataDirectory)
                        .getChildFile (JUCEApplication::getInstance()->getApplicationName())),
      hpcf_dir (app_data_dir.getChildFile ("hpcfs")),
      hpcf_url ("https://raw.githubusercontent.com/ShanonPearce/ASH-IR-Dataset/master/HpCFs/")
{
    if (! app_data_dir.exists())
        app_data_dir.createDirectory();

    if (! hpcf_dir.exists())
        hpcf_dir.createDirectory();
}
