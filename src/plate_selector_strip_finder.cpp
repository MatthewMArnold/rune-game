#include "plate_selector_strip_finder.hpp"

#include "aixlog.hpp"
#include "filters.hpp"
#include "settings.hpp"
#include "utils.hpp"

namespace rune
{
static bool isValidPlateSelectorStripContour(const std::vector<cv::Point> &contour);

void findPlateSelectorStrip(
    const cv::Mat &img,
    const std::vector<cv::RotatedRect> &candidateArmorPlates,
    const std::vector<cv::RotatedRect> &petals,
    std::vector<cv::RotatedRect> *plateSelectorStrips,
    cv::RotatedRect *petalAssociatedWithSelectorStrip)
{
    assert(plateSelectorStrips != nullptr);
    assert(petalAssociatedWithSelectorStrip != nullptr);

    if (img.empty())
    {
        return;
    }

    cv::Mat imgBinarized;
    imgBinarized = img.clone();

    // convert to BGR so we can add lines to the image
    if (imgBinarized.type() == CV_8UC1)
    {
        cv::cvtColor(imgBinarized, imgBinarized, cv::COLOR_GRAY2BGR);
    }

    std::vector<cv::RotatedRect> enlargedCandidatearmorTargets = candidateArmorPlates;
    for (auto &candidate_target_armor : enlargedCandidatearmorTargets)
    {
        cv::Point2f vertices[4];
        candidate_target_armor.size.height *=
            (float)PLATESELECTORSTRIP_SETTING("armorPlateRemovalHWExpansion");
        candidate_target_armor.size.width *=
            (float)PLATESELECTORSTRIP_SETTING("armorPlateRemovalHWExpansion");
        candidate_target_armor.points(vertices);

        for (int i = 0; i < 4; i++)
        {
            // add lines where the armor plate candidates are, colored black, "blotting" out the
            // armor plates
            cv::line(imgBinarized, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 0, 0), 20);
        }
    }

    if ((int)DEBUG_SETTING("showPlateSelectorPre"))
    {
        cv::imshow("armor plates removed", imgBinarized);
    }

    // convert back to grayscale
    cv::cvtColor(imgBinarized, imgBinarized, cv::COLOR_BGR2GRAY);

    filterPlateSelectorStripImage(imgBinarized);

    if ((int)DEBUG_SETTING("showPlateSelectorPre"))
    {
        cv::imshow("binarized armor plates removed", imgBinarized);
    }

    std::vector<std::vector<cv::Point> > contours;
    findContours(imgBinarized, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    if ((int)DEBUG_SETTING("showPlateSelectorPre"))
    {
        cv::Mat imgCpy;
        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        cv::drawContours(imgCpy, contours, DEBUG_SETTING("contourIndex"), cv::Scalar(0, 255, 0), 1);
        cv::imshow("strip contours", imgCpy);
    }

    for (auto &selectorStripContour : contours)
    {
        if (!isValidPlateSelectorStripContour(selectorStripContour))
        {
            continue;
        }

        cv::RotatedRect contourRect = cv::minAreaRect(selectorStripContour);

        for (const auto &candidatePlateSelectorStrip : petals)
        {
            std::vector<cv::Point2f> intersection;
            if (!cv::rotatedRectangleIntersection(
                    contourRect,
                    candidatePlateSelectorStrip,
                    intersection))
            {
                continue;
            }

            float areaIntersectingRatio = cv::contourArea(intersection) / contourRect.size.area();

            if (areaIntersectingRatio >
                (float)PLATESELECTORSTRIP_SETTING("minAreaIntersectingRatio"))
            {
                plateSelectorStrips->emplace_back(cv::minAreaRect(selectorStripContour));
                *petalAssociatedWithSelectorStrip = candidatePlateSelectorStrip;
            }
        }
    }

    if ((int)DEBUG_SETTING("showPlateSelectorPre"))
    {
        cv::Mat imgCpy;
        cv::cvtColor(img, imgCpy, cv::COLOR_GRAY2BGR);
        for (const auto &rect : *plateSelectorStrips)
        {
            drawRotatedRect(imgCpy, rect, cv::Scalar(0, 255, 0));
        }
        cv::imshow("plate selector strips", imgCpy);
    }
}

static bool isValidPlateSelectorStripContour(const std::vector<cv::Point> &contour)
{
    double contourArea = cv::contourArea(contour);
    if (contourArea > (float)PLATESELECTORSTRIP_SETTING("contourAreaMax") ||
        contourArea < (float)PLATESELECTORSTRIP_SETTING("contourAreaMin"))
    {
        LOG(INFO) << "contour area " << contourArea << std::endl;
        return false;
    }

    cv::RotatedRect contourRect = minAreaRect(contour);
    cv::Size2f contourSize = contourRect.size;

    float h = contourSize.height > contourSize.width ? contourSize.height : contourSize.width;
    float w = contourSize.height < contourSize.width ? contourSize.height : contourSize.width;

    if (h < (float)PLATESELECTORSTRIP_SETTING("contourLengthMin") ||
        w < (float)PLATESELECTORSTRIP_SETTING("contourWidthMin") ||
        h > (float)PLATESELECTORSTRIP_SETTING("contourLengthMax") ||
        w > (float)PLATESELECTORSTRIP_SETTING("contourWidthMax"))
    {
        LOG(INFO) << "h " << h << " w " << w << std::endl;
        return false;
    }

    float hwRatio = h / w;

    if (hwRatio > (float)PLATESELECTORSTRIP_SETTING("contourLWRatioMax") ||
        hwRatio < (float)PLATESELECTORSTRIP_SETTING("contourLWRatioMin"))
    {
        LOG(INFO) << "hwRatio " << hwRatio << std::endl;
        return false;
    }

    if (contourArea / contourSize.area() < (float)PLATESELECTORSTRIP_SETTING("contourAreaRatioMin"))
    {
        LOG(INFO) << "contour area ratio " << (contourArea / contourSize.area()) << std::endl;
        return false;
    }

    LOG(INFO) << "area " << contourArea << " h " << h << "  w " << w << " hwRatio " << hwRatio
              << " contour area ratio " << (contourArea / contourSize.area()) << std::endl;

    return true;
}
}  // namespace rune
