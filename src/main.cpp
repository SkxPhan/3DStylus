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
public:
    VideoPlayer(std::string_view directoryPath, int frameRate) : directoryPath(directoryPath), frameRate(frameRate) {}

    void playDepthVideo()
    {
        cv::String imagePattern = directoryPath + "*_depth.tiff";

        std::vector<cv::String> imageFilenames;
        cv::glob(imagePattern, imageFilenames);

        if (imageFilenames.empty())
        {
            std::cerr << "Error: No images found in the specified directory.\n";
            return;
        }

        ImageDisplay imageDisplay;

        int delay = 1000 / frameRate;

        for (const cv::String &imageFilename : imageFilenames)
        {
            cv::Mat image{cv::imread(imageFilename)};
            imageDisplay.play(image);
            cv::waitKey(delay);
        }
    }

    void play(const cv::Mat &frame) const override
    {
        cv::imshow("Player", frame);
    }

private:
    std::string directoryPath;
    int frameRate;
};

int main()
{
    std::string directoryPath = "../data/ds325/gestures_two_hands/";
    int frameRate = 30;

    VideoPlayer videoPlayer(directoryPath, frameRate);
    videoPlayer.playDepthVideo();

    return 0;
}