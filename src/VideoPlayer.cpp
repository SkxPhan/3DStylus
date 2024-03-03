#include "VideoPlayer.hpp"

VideoPlayer::VideoPlayer(int frameRate) : frameRate(frameRate) {}

void VideoPlayer::play(const std::string &path) {
  std::vector<cv::String> imageFilenames;
  cv::glob(path, imageFilenames);

  if (imageFilenames.empty()) {
    std::cerr << "Error: No images found in the specified directory.\n";
    return;
  }

  std::cout << "Playing video...\n";

  int delayInMS = 1000 / frameRate;

  for (const cv::String &imageFilename : imageFilenames) {
    cv::Mat image{cv::imread(imageFilename)};
    cv::imshow("Video Player", image);
    cv::waitKey(delayInMS);
  }
  std::cout << "End of video." << std::endl;
}