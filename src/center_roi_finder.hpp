#pragma once

#include <opencv2/opencv.hpp>

namespace rune
{
void findCenterROI(const cv::Mat &img, cv::RotatedRect *centerROI);
}
