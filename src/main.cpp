#include <opencv2/opencv.hpp>
#include <string_view>

#include "ImageProcessing.hpp"
#include "StrokeDetector.hpp"

using namespace cv;

int main(int argc, char* argv[]) {
  const std::string sequencePath = "../data/ds536/rectangle_cw";
  std::vector<std::string> imageFiles;
  glob(sequencePath + "/*depth.tif", imageFiles);

  const std::string_view strokeTemplatePath = "tests/testdata";
  StrokeDetector detector;
  detector.loadTemplateStrokes(strokeTemplatePath);

  for (const auto& imageFile : imageFiles) {
    Mat frame = cv::imread(imageFile, cv::IMREAD_GRAYSCALE);

    Mat handFrame;
    detectForegroundHand(frame, handFrame);

    cv::Point centroid = getCentroid(handFrame, false);
    detector.appendToBufferAndDetectStroke({centroid.x, centroid.y});
    drawCentroid(frame, centroid);

    Mat combinedFrame;
    mergeFrames({frame, handFrame}, combinedFrame);
    imshow("Depth video", combinedFrame);

    if (waitKey(1000 / 30) == 27) break;
  }

  destroyAllWindows();

  return 0;
}