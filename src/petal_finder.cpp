#include "petal_finder.hpp"

#include "aixlog.hpp"
#include "filters.hpp"
#include "settings.hpp"
#include "utils.hpp"

namespace rune
{
static bool isValidPetal(const cv::Mat &img, const std::vector<cv::Point> &petal);

static float computePercentColored(const cv::Mat &img, const cv::RotatedRect &boundingBox);

void findPetals(const cv::Mat &img, std::vector<cv::RotatedRect> *petals, int *prevPetalsFound)
{
    assert(petals != nullptr);
    assert(prevPetalsFound != nullptr);

    if (img.empty())
    {
        return;
    }

    cv::Mat imgProcessed;
    imgProcessed = img.clone();

    if (img.type() == CV_8UC3)
    {
        cv::cvtColor(imgProcessed, imgProcessed, cv::COLOR_BGR2GRAY);
    }

    // apply filtering to grayscale image and find external contours
    std::vector<std::vector<cv::Point> > contours;
    filterPetalImage(imgProcessed);
    cv::findContours(imgProcessed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    // add valid petals to the list
    for (auto &petalContour : contours)
    {
        if (!isValidPetal(imgProcessed, petalContour))
        {
            continue;
        }
        petals->emplace_back(cv::minAreaRect(petalContour));
    }

    if ((int)DEBUG_SETTING("showPetalPre"))
    {
        cv::imshow("petal struct", imgProcessed);

        cv::Mat imgCpy;
        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        cv::drawContours(imgCpy, contours, DEBUG_SETTING("contourIndex"), cv::Scalar(0, 255, 0), 1);
        cv::imshow("petal contours", imgCpy);

        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        for (const auto &petal : *petals)
        {
            drawRotatedRect(imgCpy, petal, cv::Scalar(0, 255, 0));
        }
        cv::imshow("petals", imgCpy);
    }

    if (petals->size() < *prevPetalsFound)
    {
        LOG(ERROR) << "less petals found than before, detection failed " << std::endl;
    }

    *prevPetalsFound = petals->size();
}

static bool isValidPetal(const cv::Mat &, const std::vector<cv::Point> &petal)
{
    float contourArea = cv::contourArea(petal);

    cv::RotatedRect boundingRect = cv::minAreaRect(petal);
    cv::Size2f boundingSize = boundingRect.size;

    float boundingArea = boundingSize.area();

    if (boundingArea < (float)PETALDETECTOR_SETTING("boundingAreaMin") ||
        boundingArea > (float)PETALDETECTOR_SETTING("boundingAreaMax"))
    {
        LOG(INFO) << "bounding area " << boundingArea << std::endl;
    }

    float h = boundingSize.height > boundingSize.width ? boundingSize.height : boundingSize.width;
    float w = boundingSize.height < boundingSize.width ? boundingSize.height : boundingSize.width;

    if (h < (float)PETALDETECTOR_SETTING("contourHeightMin") ||
        w < (float)PETALDETECTOR_SETTING("contourWidthMin") ||
        h > (float)PETALDETECTOR_SETTING("contourHeightMax") ||
        w > (float)PETALDETECTOR_SETTING("contourWidthMax"))
    {
        LOG(INFO) << "h " << h << " w " << w << std::endl;
        return false;
    }

    float hwRatio = h / w;

    if (hwRatio > (float)PETALDETECTOR_SETTING("contourHWRatioMax") ||
        hwRatio < (float)PETALDETECTOR_SETTING("contourHWRatioMin"))
    {
        LOG(INFO) << "lwratio " << hwRatio << std::endl;
        return false;
    }

    if (contourArea / boundingSize.area() <
        (float)PETALDETECTOR_SETTING("contourAreaRatioMin"))
    {
        LOG(INFO) << "contour area bb area ratio " << (contourArea / boundingSize.area())
                  << std::endl;
        return false;
    }

    // consider using this in the future
    // float percentColored = computePercentColored(img, boundingRect);

    LOG(INFO) << "bounding area " << boundingArea << " h " << h << " w " << w << " hwRatio "
              << hwRatio << " area ratio " << (contourArea / boundingSize.area()) << std::endl;

    return true;
}

static float computePercentColored(const cv::Mat &img, const cv::RotatedRect &boundingBox)
{
    return 0;
}
}  // namespace rune
