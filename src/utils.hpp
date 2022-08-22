#pragma once

#include <opencv2/opencv.hpp>

/**
 * Find the center between two points
 */
inline cv::Point2f center(const cv::Point2f &p1, const cv::Point2f &p2)
{
    return cv::Point2f((p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f);
}

inline void drawRotatedRect(cv::InputOutputArray out, cv::RotatedRect rect, cv::Scalar color)
{
    cv::Point2f vertices[4];
    rect.points(vertices);
    for (int i = 0; i < 4; i++)
    {
        cv::line(out, vertices[i], vertices[(i + 1) % 4], color, 2);
    }
}
