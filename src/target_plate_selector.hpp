#pragma once

#include <opencv2/opencv.hpp>

namespace rune
{
void findTargetPlate(
    const cv::RotatedRect &plateSelectorStrip,
    const std::vector<cv::RotatedRect> &candidateArmorPlates,
    const cv::RotatedRect &petalAssociatedWithSelectorStrip,
    cv::RotatedRect *armorTarget);
}  // namespace rune
