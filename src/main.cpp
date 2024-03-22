#include <opencv2/opencv.hpp>
#include <string_view>

#include "ImageProcessing.hpp"
#include "StrokeDetector.hpp"
#include "dollar.hpp"

using namespace cv;

int main(int argc, char* argv[]) {
  const std::string sequencePath = "../data/ds536/rectangle_cw";
  std::vector<std::string> imageFiles;
  glob(sequencePath + "/*depth.tif", imageFiles);

  const std::string_view strokeTemplatePath = "tests/testdata";
  StrokeDetector detector;
  detector.loadTemplateStrokes(strokeTemplatePath);

  std::vector<dollar::Point> sample;

  for (const auto& imageFile : imageFiles) {
    Mat frame = cv::imread(imageFile, cv::IMREAD_GRAYSCALE);

    Mat invertedFrame;
    bitwise_not(frame, invertedFrame);

    Mat openedFrame;
    applyMorphologicalOperation(invertedFrame, openedFrame, cv::MORPH_OPEN, 5);

    Mat foregroundMask;
    removeBackgroundDepthPixels(openedFrame, foregroundMask);

    Mat maskedFrame;
    bitwise_and(invertedFrame, invertedFrame, maskedFrame, foregroundMask);

    Mat handFrame;
    applyHandSegmentation(maskedFrame, handFrame);

    cv::Point centroid = getCentroid(handFrame, false);
    drawCentroid(frame, centroid);

    if (centroid.x || centroid.y) {
      sample.push_back({centroid.x, centroid.y});
    } else {
      sample.clear();
    }
    if (!isMoving(sample, 15)) {
      if (sample.size() > 80) {
        dollar::Stroke testStroke{sample, dollar::Orientation::Sensitive};
        std::cout << "Detected stroke: " << detector.recognize(testStroke)
                  << "\n";
      }
      sample.clear();
    }

    Mat combinedFrame;
    hconcat(frame, foregroundMask, combinedFrame);
    hconcat(combinedFrame, handFrame, combinedFrame);
    imshow("Depth video", combinedFrame);

    if (waitKey(1000 / 30) == 27) break;
  }

  destroyAllWindows();

  return 0;
}