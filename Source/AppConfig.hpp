#pragma once

#include <JuceHeader.h>

class AppConfig
{
private:
    AppConfig();

    juce::File app_data_dir;
    juce::File hpcf_dir;

    juce::String hpcf_url;

public:
    static const AppConfig& Instance()
    {
        static AppConfig conf;
        return conf;
    }

    AppConfig (const AppConfig&) = delete;
    AppConfig& operator= (const AppConfig&) = delete;
    AppConfig (AppConfig&&) = delete;
    AppConfig& operator= (AppConfig&&) = delete;

    const juce::File& getAppDataDir() const { return app_data_dir; }
    const juce::File& getHPCFDir() const { return hpcf_dir; }
    const juce::String getHPCFURL() const { return hpcf_url; }
};
