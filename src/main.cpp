#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::string directoryPath = "../data/ds325/gestures_two_hands/";

    cv::String imagePattern = directoryPath + "*_depth.tiff";

    std::vector<cv::String> imageFilenames;

    cv::glob(imagePattern, imageFilenames, false);

    if (imageFilenames.empty())
    {
        std::cerr << "Error: No images found in the specified directory.\n";
        return 1;
    }

    int delay = 1000 / 30;

    cv::Mat image;

    for (const cv::String &imageFilename : imageFilenames)
    {
        image = cv::imread(imageFilename);
        cv::imshow("Images", image);
        cv::waitKey(delay);
    }
}