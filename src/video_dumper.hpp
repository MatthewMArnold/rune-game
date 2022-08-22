#pragma once

#include <opencv2/opencv.hpp>

namespace rune
{
class VideoDumper
{
public:
    static inline VideoDumper *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new VideoDumper();
        }
        return instance;
    }

    VideoDumper();

    void initialize(const std::string vidName, const cv::VideoCapture &vidCapture);

    void write(const cv::Mat &frame);

private:
    static VideoDumper *instance;

    cv::VideoWriter writer;
};
}  // namespace rune
