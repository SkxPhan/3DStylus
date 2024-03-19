#include <opencv2/opencv.hpp>
#include <string_view>

#include "ImageProcessing.hpp"
#include "StrokeDetector.hpp"
#include "dollar.hpp"

using namespace cv;

int main(int argc, char* argv[]) {
  const std::string_view path = "../data/ds536/rectangle_cw/";
  const std::string_view extension = ".tif";

  auto i = 0;

  StrokeDetector detector("../tests/testdata");
  std::vector<std::pair<float, float>> sample;

  while (true) {
    std::ostringstream filename;
    filename << path << std::setw(6) << std::setfill('0') << i << "_depth"
             << extension;
    Mat frame = cv::imread(filename.str(), cv::IMREAD_GRAYSCALE);

    if (frame.empty()) break;

    bitwise_not(frame, frame);
    cv::threshold(frame, frame, 200, 0, cv::THRESH_TOZERO);

    Mat clean_frame;
    applyMorphologicalOperation(frame, clean_frame, cv::MORPH_OPEN, 5);

    // Remove far pixels
    Mat thresholdedImage;
    removeBackgroundDepthPixels(clean_frame, thresholdedImage);

    Mat maskedImage;
    bitwise_and(frame, frame, maskedImage, thresholdedImage);

    // Calculate histogram
    Mat hist;
    getHistogramDepthPixels(maskedImage, hist);
    auto maxIntensity = findIntensityWithHighestFrequency(hist);
    threshold(maskedImage, maskedImage, maxIntensity, 255, THRESH_BINARY);
    applyMorphologicalOperation(maskedImage, maskedImage, cv::MORPH_CLOSE, 3);

    // Find and draw centroid
    cv::Point centroid = getCentroid(maskedImage, false);
    drawCentroid(frame, centroid);
    if (centroid.x || centroid.y) {
      sample.push_back({centroid.x, centroid.y});
    } else {
      sample.clear();
    }
    if (!isMoving(sample, 15)) {
      if (sample.size() > 80) {
        std::cout << "Analyzing stroke...\n";
        dollar::Stroke testStroke{sample, dollar::Orientation::Sensitive};
        std::cout << detector.recognize(testStroke) << "\n";
        for (const auto& point : sample) {
          std::cout << "(" << point.first << ", " << point.second << "), ";
        }
        std::cout << "\nSize: " << sample.size() << "\n\n";
        sample.clear();
      } else {
        sample.clear();
      }
    }

    Mat combinedFrame;
    hconcat(frame, thresholdedImage, combinedFrame);
    hconcat(combinedFrame, maskedImage, combinedFrame);
    imshow("Depth video", combinedFrame);
    if (waitKey(1000 / 30) == 27) break;

    i++;
  }

  destroyAllWindows();

  return 0;
}