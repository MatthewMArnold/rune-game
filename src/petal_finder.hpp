#pragma once

#include <opencv2/opencv.hpp>

namespace rune {
void findPetals(const cv::Mat &img, std::vector<cv::RotatedRect> *petals, int *prevPetalsFound);
}
