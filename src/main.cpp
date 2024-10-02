#include <opencv2/opencv.hpp>

#include "ImageProcessing.hpp"
#include "StrokeDetector.hpp"

int main(int argc, char* argv[]) {
  const std::string sequencePath = "../data/ds536/rectangle_cw";
  std::vector<std::string> imageFiles;
  cv::glob(sequencePath + "/*depth.tif", imageFiles);

  const std::string_view strokeTemplatePath = "../data/templates";
  StrokeDetector detector;
  detector.loadTemplateStrokes(strokeTemplatePath);

  for (const auto& imageFile : imageFiles) {
    cv::Mat frame = cv::imread(imageFile, cv::IMREAD_GRAYSCALE);

    cv::Mat handFrame;
    detectForegroundHand(frame, handFrame);

    cv::Point centroid = getCentroid(handFrame, false);
    detector.appendToBufferAndDetectStroke({centroid.x, centroid.y});
    drawCentroid(frame, centroid);

    cv::Mat combinedFrame;
    mergeFrames({frame, handFrame}, combinedFrame);
    cv::imshow("Depth video", combinedFrame);

    if (cv::waitKey(1000 / 30) == 27) break;
  }

  cv::destroyAllWindows();

  return 0;
}
