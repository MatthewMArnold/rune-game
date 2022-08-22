#include "plate_finder.hpp"

#include "aixlog.hpp"
#include "filters.hpp"
#include "settings.hpp"
#include "utils.hpp"

namespace rune
{
static bool isValidPlateContour(const std::vector<cv::Point> &plateContour);

void findPlates(const cv::Mat &img, std::vector<cv::RotatedRect> *armors)
{
    assert(armors != nullptr);

    cv::Mat imgProcessed;
    img.copyTo(imgProcessed);

    if (img.type() == CV_8UC3)
    {
        cv::cvtColor(imgProcessed, imgProcessed, cv::COLOR_BGR2GRAY);
    }

    filterPlateImage(imgProcessed);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(imgProcessed, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

    if ((int)DEBUG_SETTING("showArmorPlatePre"))
    {
        cv::Mat imgCpy;
        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        cv::drawContours(imgCpy, contours, -1, cv::Scalar(0, 255, 0));
        cv::imshow("all contours", imgCpy);
    }

    // remove external contours from all contours (these cannot possibly be plates)
    for (size_t i = 0; i < hierarchy.size();)
    {
        cv::Vec4i &v = hierarchy[i];

        if (v[3] == -1)  // doesn't have children
        {
            std::swap(hierarchy[i], hierarchy[hierarchy.size() - 1]);
            hierarchy.pop_back();

            std::swap(contours[i], contours[contours.size() - 1]);
            contours.pop_back();
        }
        else
        {
            i++;
        }
    }

    for (auto &plateContour : contours)
    {
        if (!isValidPlateContour(plateContour))
        {
            continue;
        }
        armors->emplace_back(cv::minAreaRect(plateContour));
    }

    if ((int)DEBUG_SETTING("showArmorPlatePre"))
    {
        cv::imshow("armor struct", imgProcessed);

        cv::Mat imgCpy;
        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        cv::drawContours(imgCpy, contours, DEBUG_SETTING("contourIndex"), cv::Scalar(0, 255, 0), 1);
        cv::imshow("internal plate contours", imgCpy);

        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        for (const auto &armor : *armors)
        {
            drawRotatedRect(imgCpy, armor, cv::Scalar(0, 255, 0));
        }
        cv::imshow("armor plates", imgCpy);
    }
}

static bool isValidPlateContour(const std::vector<cv::Point> &plateContour)
{
    float contourArea = cv::contourArea(plateContour);

    cv::RotatedRect boundingRect = minAreaRect(plateContour);
    cv::Size2f boundingSize = boundingRect.size;

    float h = boundingSize.height > boundingSize.width ? boundingSize.height : boundingSize.width;
    float w = boundingSize.height < boundingSize.width ? boundingSize.height : boundingSize.width;
    float hwRatio = h / w;

    if (contourArea > (float)PLATEDETECTOR_SETTING("conourAreaMax") ||
        contourArea < (float)PLATEDETECTOR_SETTING("contourAreaMin"))
    {
        LOG(INFO) << "plate area " << contourArea << std::endl;
        return false;
    }

    if (h < (float)PLATEDETECTOR_SETTING("contourLengthMin") ||
        w < (float)PLATEDETECTOR_SETTING("contourWidthMin") ||
        h > (float)PLATEDETECTOR_SETTING("contourLengthMax") ||
        w > (float)PLATEDETECTOR_SETTING("contourWidthMax"))
    {
        LOG(INFO) << "plate h " << h << " w " << w << std::endl;
        return false;
    }

    if (hwRatio > (float)PLATEDETECTOR_SETTING("contourHWRatioMax") ||
        hwRatio < (float)PLATEDETECTOR_SETTING("contourHWRatioMin"))
    {
        LOG(INFO) << "lw ratio " << hwRatio << std::endl;
        return false;
    }

    if (contourArea / boundingSize.area() <
        (float)PLATEDETECTOR_SETTING("contourAreaBBAreaRatioMin"))
    {
        LOG(INFO) << "contour area bb area ratio " << (contourArea / boundingSize.area())
                  << std::endl;
        return false;
    }

    LOG(INFO) << "plate area " << contourArea << " plate h " << h << " w " << w
              << " lw ratio " << hwRatio << " contour area bb area ratio "
              << (contourArea / boundingSize.area()) << std::endl;

    return true;
}
}  // namespace rune
