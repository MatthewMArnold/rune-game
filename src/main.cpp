#include <iostream>

#include "opencv2/opencv.hpp"

#include "aixlog.hpp"
#include "binarize_image.hpp"
#include "center_finder.hpp"
#include "center_roi_finder.hpp"
#include "petal_finder.hpp"
#include "plate_finder.hpp"
#include "plate_selector_strip_finder.hpp"
#include "settings.hpp"
#include "target_plate_selector.hpp"
#include "utils.hpp"
#include "video_dumper.hpp"

using namespace rune;

static void drawResult(
    cv::Mat &img,
    const std::vector<cv::RotatedRect> &plates,
    const std::vector<cv::RotatedRect> &petals,
    const std::vector<cv::RotatedRect> &strips,
    const cv::RotatedRect &center)
{
    for (const auto &plate : plates)
    {
        drawRotatedRect(img, plate, cv::Scalar(0, 255, 0));
    }
    for (const auto &petal : petals)
    {
        drawRotatedRect(img, petal, cv::Scalar(0, 255, 255));
    }
    for (const auto &strip : strips)
    {
        drawRotatedRect(img, strip, cv::Scalar(120, 255, 0));
    }
    drawRotatedRect(img, center, cv::Scalar(0, 0, 255));
}

static void processFrame(cv::Mat &frame, cv::Mat *outputFrame)
{
    assert(outputFrame != nullptr);

    cv::Mat savedFrame = frame.clone();

    cv::RotatedRect centerROI;
    rune::findCenterROI(frame, &centerROI);

    binarizeInputImage(frame);

    cv::RotatedRect armorTarget;

    std::vector<cv::RotatedRect> plates;
    rune::findPlates(frame, &plates);

    std::vector<cv::RotatedRect> petals;
    int prevPetalsFound = 0;
    rune::findPetals(frame, &petals, &prevPetalsFound);

    std::vector<cv::RotatedRect> strips;
    cv::RotatedRect petalAssociatedWithSelectorStrip;
    rune::findPlateSelectorStrip(frame, plates, petals, &strips, &petalAssociatedWithSelectorStrip);

    findTargetPlate(strips[0], plates, petalAssociatedWithSelectorStrip, &armorTarget);

    cv::RotatedRect center;
    rune::findCenter(frame, armorTarget, centerROI, &center);

    drawResult(*outputFrame, plates, petals, strips, center);

    if ((int)DEBUG_SETTING("showFinalResult"))
    {
        cv::Mat target = savedFrame.clone();
        drawRotatedRect(target, armorTarget, cv::Scalar(0, 0, 255));
        cv::imshow("target plate", target);
        drawRotatedRect(*outputFrame, armorTarget, cv::Scalar(0, 0, 255));
        cv::imshow("final result", *outputFrame);
    }

    cv::waitKey();
}

int main(int argv, char **argc)
{
    if (argv < 2)
    {
        LOG(FATAL) << "must specify testing type, either -v or -i" << std::endl;
        return 1;
    }

    std::string arg2 = argc[1];
    if (arg2 != "-v" && arg2 != "-i")
    {
        LOG(FATAL) << "must specify testing type, either -v or -i" << std::endl;
        return 1;
    }

    bool testVideo = arg2 == "-v";

    std::string masterConfigurationFile = (argv == 3) ? argc[2] : "";

    if (masterConfigurationFile.size() != 0)
    {
        Settings::getInstance()->setMasterConfigurationFile(masterConfigurationFile);
    }

    AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::info);

    Settings::getInstance()->loadSettings();

    AixLog::Log::init<AixLog::SinkCout>(
        static_cast<AixLog::Severity>((int)DEBUG_SETTING("logLevel")));

    if (testVideo)
    {
        std::string videoFile = VIDEOCONFIG_SETTING("videoFile").string();
        std::string videoFN = Settings::getInstance()->getSettingsPath() + "/" + videoFile;
        std::string videoDumperFN =
            videoFile.substr(0, videoFile.find_last_of(".")) + "_result.mkv";

        cv::VideoCapture capture = cv::VideoCapture(videoFN, cv::CAP_FFMPEG);

        rune::VideoDumper::getInstance()->initialize(
            Settings::getInstance()->getSettingsPath() + "/" + videoDumperFN,
            capture);

        if (!capture.isOpened())
        {
            LOG(FATAL) << "error in opening video capture";
            assert(false);
        }

        while (capture.isOpened())
        {
            cv::Mat frame;
            if (capture.read(frame))
            {
                cv::Mat outputFrame = frame.clone();
                processFrame(frame, &outputFrame);
                rune::VideoDumper::getInstance()->write(outputFrame);
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        cv::Mat frame = imread(VIDEOCONFIG_SETTING("imageFile"));
        cv::Mat outputFrame = frame.clone();
        processFrame(frame, &outputFrame);
    }
}
