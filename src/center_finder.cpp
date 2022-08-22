#include "center_finder.hpp"

#include <opencv2/opencv.hpp>

#include "aixlog.hpp"
#include "filters.hpp"
#include "settings.hpp"
#include "utils.hpp"

namespace rune
{
static bool isValidCenterRContour(
    const std::vector<cv::Point> &contour,
    const cv::RotatedRect &centerROI);

void findCenter(
    const cv::Mat &img,
    const cv::RotatedRect &armorTarget,
    const cv::RotatedRect &centerROI,
    cv::RotatedRect *center)
{
    assert(center != nullptr);

    if (img.empty())
    {
        return;
    }

    cv::Mat imgBinarized;
    imgBinarized = img.clone();
    if (img.type() == CV_8UC3)
    {
        cv::cvtColor(imgBinarized, imgBinarized, cv::COLOR_BGR2GRAY);
    }

    filterCenterImage(imgBinarized);

    if ((int)DEBUG_SETTING("showCenterPre"))
    {
        cv::imshow("binarized center R", imgBinarized);
    }

    std::vector<std::vector<cv::Point> > contours;

    cv::findContours(imgBinarized, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    if ((int)DEBUG_SETTING("showCenterPre"))
    {
        cv::Mat imgCpy;
        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        cv::drawContours(imgCpy, contours, DEBUG_SETTING("contourIndex"), cv::Scalar(0, 255, 0), 1);
        cv::imshow("center R contours", imgCpy);
    }

    cv::Mat imgCpy;
    cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);

    for (auto &contour : contours)
    {
        if (!isValidCenterRContour(contour, centerROI))
        {
            continue;
        }
        drawRotatedRect(imgCpy, cv::minAreaRect(contour), cv::Scalar(0, 255, 0));
        *center = cv::minAreaRect(contour);
        // break;
    }

    if ((int)DEBUG_SETTING("showCenterPre"))
    {
        cv::imshow("centers found", imgCpy);
    }
}

static bool isValidCenterRContour(
    const std::vector<cv::Point> &contour,
    const cv::RotatedRect &centerROI)
{
    double contourArea = cv::contourArea(contour);
    if (contourArea > (float)RUNECENTER_SETTING("contourAreaMax") ||
        contourArea < (float)RUNECENTER_SETTING("contourAreaMin"))
    {
        LOG(INFO) << "contour area " << contourArea << std::endl;
        return false;
    }

    cv::RotatedRect contourRect = minAreaRect(contour);
    cv::Size2f contourRectSize = contourRect.size;

    float h = contourRectSize.height > contourRectSize.width ? contourRectSize.height
                                                             : contourRectSize.width;
    float w = contourRectSize.height < contourRectSize.width ? contourRectSize.height
                                                             : contourRectSize.width;

    if (h < (float)RUNECENTER_SETTING("contourLengthMin") ||
        w < (float)RUNECENTER_SETTING("contourWidthMin") ||
        h > (float)RUNECENTER_SETTING("contourLengthMax") ||
        w > (float)RUNECENTER_SETTING("contourWidthMax"))
    {
        LOG(INFO) << "h " << h << " w " << w << std::endl;
        return false;
    }

    float hwRatio = h / w;
    if (hwRatio > (float)RUNECENTER_SETTING("contourLWRatioMax") ||
        hwRatio < (float)RUNECENTER_SETTING("contourLWRatioMin"))
    {
        LOG(INFO) << "hwRatio " << hwRatio << std::endl;
        return false;
    }

    if (contourArea / contourRectSize.area() < (int)RUNECENTER_SETTING("contourAreaBBRatioMin"))
    {
        LOG(INFO) << "contour area BB ratio " << (contourArea / contourRectSize.area())
                  << std::endl;
        return false;
    }

    std::vector<cv::Point2f> intersection;
    if (!cv::rotatedRectangleIntersection(contourRect, centerROI, intersection))
    {
        LOG(INFO) << "doesn't intersect center ROI" << std::endl;
        return false;
    }

    float areaIntersectingRatio = cv::contourArea(intersection) / contourRect.size.area();

    if (areaIntersectingRatio < (float)RUNECENTER_SETTING("areaIntersectingRatio"))
    {
        LOG(INFO) << "intersection area " << areaIntersectingRatio << std::endl;
        return false;
    }

    LOG(INFO) << "location " << cv::boundingRect(contour).x << ", " << cv::boundingRect(contour).y
              << " contour area " << contourArea << " h " << h << " w " << w << " hwRatio "
              << hwRatio << " intersection area " << cv::contourArea(intersection) << std::endl;

    return true;
}
}  // namespace rune
