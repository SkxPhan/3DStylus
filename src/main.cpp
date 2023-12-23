#include <opencv2/opencv.hpp>
#include <iostream>

class MediaPlayer
{
public:
    virtual void play(const cv::Mat &frame) const = 0;
    virtual ~MediaPlayer() {}
};

class ImageDisplay : public MediaPlayer
{
public:
    void play(const cv::Mat &frame) const override
    {
        cv::imshow("Player", frame);
    }
};

class VideoPlayer : public MediaPlayer
{
private:
    cv::String imagePattern;
    int frameRate;

public:
    VideoPlayer(cv::String imagePattern, int frameRate) : imagePattern(imagePattern), frameRate(frameRate) {}

    void playDepthVideo()
    {
        std::vector<cv::String> imageFilenames;
        cv::glob(imagePattern, imageFilenames);

        if (imageFilenames.empty())
        {
            std::cerr << "Error: No images found in the specified directory.\n";
            return;
        }

        ImageDisplay imageDisplay;

        int delayInMS = 1000 / frameRate;

        for (const cv::String &imageFilename : imageFilenames)
        {
            cv::Mat image{cv::imread(imageFilename)};
            imageDisplay.play(image);
            cv::waitKey(delayInMS);
        }
    }

    void play(const cv::Mat &frame) const override
    {
        cv::imshow("Player", frame);
    }
};

int main()
{
    std::string directoryPath = "../data/ds325/gestures_two_hands/";
    std::string filenamePattern = "*_depth.tiff";
    cv::String imagePattern = directoryPath + filenamePattern;
    int frameRate = 30;

    VideoPlayer videoPlayer(imagePattern, frameRate);
    videoPlayer.playDepthVideo();

    return 0;
}