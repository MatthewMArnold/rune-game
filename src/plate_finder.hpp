#pragma once

#include <opencv2/opencv.hpp>

namespace rune
{
void findPlates(const cv::Mat &img, std::vector<cv::RotatedRect> * armors);
}