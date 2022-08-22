#include "binarize_image.hpp"

#include "filters.hpp"
#include "settings.hpp"

namespace rune
{
void binarizeInputImage(cv::Mat &img)
{
    if (img.type() != CV_8UC3)
    {
        return;
    }

    std::vector<cv::Mat> splitChannels;
    cv::split(img, splitChannels);

    cv::Mat colorChannelToUse;
    int grayThreshold = 0;
    int colorThreshold = 0;

    if ((int)RUNE_GAME_SETTING("detectRed"))
    {
        colorChannelToUse = splitChannels[2] - splitChannels[0];
        colorThreshold = RUNE_GAME_SETTING("colorBinaryThresholdRed");
        grayThreshold = RUNE_GAME_SETTING("grayscaleThresholdRed");
    }
    else
    {
        colorChannelToUse =  splitChannels[0] - splitChannels[2];
        colorThreshold = RUNE_GAME_SETTING("colorBinaryThresholdBlue");
        grayThreshold = RUNE_GAME_SETTING("grayscaleThresholdBlue");
    }

    filterColorChannel(colorChannelToUse);

    cv::threshold(colorChannelToUse, colorChannelToUse, colorThreshold, 255, cv::THRESH_BINARY);

    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    cv::threshold(img, img, grayThreshold, 255, cv::THRESH_BINARY);

    if ((int)DEBUG_SETTING("showPreprocessing"))
    {
        cv::imshow("gray threshold", img);
        cv::imshow("color threshold", colorChannelToUse);
    }

    img = img & colorChannelToUse;

    if ((int)DEBUG_SETTING("showPreprocessing"))
    {
        cv::imshow("final binarized image", img);
    }
}
}  // namespace rune
