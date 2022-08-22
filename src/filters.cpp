#include "filters.hpp"

namespace rune
{
void filterColorChannel(cv::Mat &src)
{
    cv::dilate(src, src, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
}

void filterPlateImage(cv::Mat &src)
{
    cv::dilate(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    cv::erode(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
}

void filterPetalImage(cv::Mat &src)
{
    cv::dilate(src, src, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    cv::erode(src, src, getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
    cv::erode(src, src, getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
    cv::erode(src, src, getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
}

void filterPlateSelectorStripImage(cv::Mat &src)
{
    cv::dilate(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    cv::erode(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
    cv::erode(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
    cv::erode(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
}

void filterCenterImage(cv::Mat &src)
{
    cv::erode(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 1)));
    cv::dilate(src, src, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)));
}

}  // namespace rune
