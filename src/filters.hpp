#pragma once

#include <opencv2/opencv.hpp>

namespace rune
{
void filterColorChannel(cv::Mat &src);
/**
 * Makes the image more blob-like to avoid detecting contours that shouldn't really exist
 */
void filterPlateImage(cv::Mat &src);
void filterPetalImage(cv::Mat &src);
void filterPlateSelectorStripImage(cv::Mat &src);
void filterCenterImage(cv::Mat &src);
}  // namespace rune
