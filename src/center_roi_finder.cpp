#include "center_roi_finder.hpp"

namespace rune
{
void findCenterROI(const cv::Mat &img, cv::RotatedRect *centerROI)
{
    assert(centerROI != nullptr);

    int ROI_X = img.cols * 0.5f;
    int ROI_Y = img.rows * 0.5f;

    centerROI->angle = 0;
    centerROI->center.x = img.cols/2;
    centerROI->center.y = img.rows/2;
    centerROI->size.width = ROI_X;
    centerROI->size.height = ROI_Y;
}
}  // namespace rune