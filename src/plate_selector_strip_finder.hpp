#pragma once

#include <opencv2/opencv.hpp>

namespace rune
{
void findPlateSelectorStrip(
    const cv::Mat &img,
    const std::vector<cv::RotatedRect> &candidateArmorPlates,
    const std::vector<cv::RotatedRect> &petals,
    std::vector<cv::RotatedRect> *plateSelectorStrips,
    cv::RotatedRect *petalAssociatedWithSelectorStrip);
}
