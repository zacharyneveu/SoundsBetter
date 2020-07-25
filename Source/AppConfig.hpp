#pragma once

#include <JuceHeader.h>

void checkForUpdate();

class AppConfig : public juce::URL::DownloadTask::Listener
{
private:
    AppConfig();

    juce::File app_data_dir;
	juce::File hpcf_dir;
    bool dl_complete;
    double dl_progress;

public:
    static const AppConfig& Instance() {
        static AppConfig conf;
        return conf;
    }

    AppConfig (const AppConfig&) = delete;
    AppConfig& operator= (const AppConfig&) = delete;
    AppConfig (AppConfig&&) = delete;
    AppConfig& operator= (AppConfig&&) = delete;

    const juce::File& getAppDataDir() const { return app_data_dir; }
    const juce::File downloadHPCF(std::string file_name) const;
    
    void finished(juce::URL::DownloadTask* task, bool success) override;
    void progress(juce::URL::DownloadTask* task, int64_t downloaded, int64_t totalLength) override;

};
