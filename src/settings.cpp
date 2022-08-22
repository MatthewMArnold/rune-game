#include "settings.hpp"

#include "aixlog.hpp"

using namespace cv;

Settings *Settings::singleton = nullptr;

#define SAFE_OPEN(fsName, filename)                                    \
    if (!fsName.isOpened())                                            \
    {                                                                  \
        LOG(FATAL) << "Failed to open file " << filename << std::endl; \
        assert(false);                                                 \
    }

Settings::Settings() : masterFilename("../metadata/masterConfiguration.yml") {}

void Settings::loadSettings()
{
    FileStorage topLevelFS(masterFilename, FileStorage::READ);

    SAFE_OPEN(topLevelFS, masterFilename);

    FileNode filenames = topLevelFS["filenames"];

    for (const auto &item : filenames)
    {
        if (item.type() != FileNode::STR)
        {
            LOG(ERROR) << "all items in master configuration file " << masterFilename
                       << " must be of type string, item named " << item.name()
                       << " not of correct type";

            assert(false);
        }

        std::string fn = getSettingsPath() + "/" + item.string();

        FileStorage fs(fn, FileStorage::READ);
        SAFE_OPEN(fs, fn);

        settingsFS[item.name()] = fs;
    }
}

const cv::FileStorage &Settings::getSetting(const std::string &name) const
{
    assert(settingsFS.count(name));
    return settingsFS.at(name);
}
