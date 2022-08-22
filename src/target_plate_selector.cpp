#include "target_plate_selector.hpp"

#include <opencv2/opencv.hpp>

#include "settings.hpp"

namespace rune
{
void findTargetPlate(
    const cv::RotatedRect &plateSelectorStrip,
    const std::vector<cv::RotatedRect> &candidateArmorPlates,
    const cv::RotatedRect &petalAssociatedWithSelectorStrip,
    cv::RotatedRect *armorTarget)
{
    for (const auto &armorPlate : candidateArmorPlates)
    {
        std::vector<cv::Point2f> intersection;
        if (!cv::rotatedRectangleIntersection(
                armorPlate,
                petalAssociatedWithSelectorStrip,
                intersection))
        {
            continue;
        }

        float areaIntersectingRatio = cv::contourArea(intersection) / armorPlate.size.area();

        if (cv::contourArea(intersection) >
            (float)PLATESELECTORSTRIP_SETTING("minAreaIntersectingRatio"))
        {
            *armorTarget = armorPlate;
        }
    }
}
}  // namespace rune
