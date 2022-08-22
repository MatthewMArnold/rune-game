#pragma once

#include <opencv2/opencv.hpp>

namespace rune
{
void findCenter(
    const cv::Mat &img,
    const cv::RotatedRect &armorTarget,
    const cv::RotatedRect &centerROI,
    cv::RotatedRect *center);
}
