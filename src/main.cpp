#include "VideoPlayer.hpp"

int main() {
  std::string directoryPath = "../data/ds325/gestures_two_hands/";
  std::string filenamePattern = "*_depth.tiff";
  auto frameRate = 30;
  VideoPlayer videoPlayer(frameRate);
  videoPlayer.play(directoryPath + filenamePattern);

  return 0;
}