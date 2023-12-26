#include "ImagePlayer.hpp"
#include "VideoPlayer.hpp"

int main()
{
    // ImagePlayer imagePlayer;
    // imagePlayer.play("../data/ds325/gestures_two_hands/000000_depth.tiff");

    std::string directoryPath = "../data/ds325/gestures_two_hands/";
    std::string filenamePattern = "*_depth.tiff";
    VideoPlayer videoPlayer(30);
    videoPlayer.play(directoryPath + filenamePattern);

    return 0;
}