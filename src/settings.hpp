#pragma once

#include <cinttypes>
#include <map>
#include <string>

#include "opencv2/core/core.hpp"

#define SETTING(configuration, name) (Settings::getInstance()->getSetting(configuration)[name])
#define DEBUG_SETTING(name) SETTING("debugConfiguration", name)
#define RUNE_GAME_SETTING(name) SETTING("runeGameDetectionConfiguration", name)
#define VIDEOCONFIG_SETTING(name) SETTING("videoConfiguration", name)
#define PLATEDETECTOR_SETTING(name) SETTING("plateDetectorConfiguration", name)
#define PETALDETECTOR_SETTING(name) SETTING("petalDetectorConfiguration", name)
#define PLATESELECTORSTRIP_SETTING(name) SETTING("plateSelectorStripConfiguration", name)
#define RUNECENTER_SETTING(name) SETTING("runeCenterConfiguration", name)

struct Settings
{
public:
    void setMasterConfigurationFile(const std::string &file) { masterFilename = file; }

    static Settings *getInstance()
    {
        if (singleton == nullptr)
        {
            singleton = new Settings();
        }
        return singleton;
    }

    Settings();

    void loadSettings();

    const cv::FileStorage &getSetting(const std::string &name) const;

    std::string getSettingsPath() const
    {
        return masterFilename.substr(0, masterFilename.find_last_of("/"));
    }

private:
    static Settings *singleton;

    std::string masterFilename;

    std::map<std::string, cv::FileStorage> settingsFS;
};
