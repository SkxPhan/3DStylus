#include "ImagePlayer.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

ImagePlayer::ImagePlayer() {}

void ImagePlayer::play(const std::string &path)
{
    cv::Mat image = cv::imread(path);
    if (image.empty())
    {
        std::cerr << "Error: Unable to open image file." << std::endl;
        return;
    }

    std::cout << "Displaying image: " << path << std::endl;
    cv::imshow("Image Player", image);
    cv::waitKey(0);
}
