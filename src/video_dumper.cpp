#include "video_dumper.hpp"

#include "aixlog.hpp"

namespace rune
{
VideoDumper *VideoDumper::instance = nullptr;

VideoDumper::VideoDumper() {}

void VideoDumper::initialize(const std::string vidName, const cv::VideoCapture &vidCapture)
{
    std::cout << vidName << std::endl;

    writer.open(
        vidName,
        cv::VideoWriter::fourcc('m','p','4','v'),
        vidCapture.get(cv::CAP_PROP_FPS),
        cv::Size(
            (int)vidCapture.get(cv::CAP_PROP_FRAME_WIDTH),
            (int)vidCapture.get(cv::CAP_PROP_FRAME_HEIGHT)),
        true);

    if (!writer.isOpened())
    {
        LOG(FATAL) << "failed to open vid capture" << std::endl;
        assert(false);
    }
}

void VideoDumper::write(const cv::Mat &frame) { writer.write(frame); }
}  // namespace rune
