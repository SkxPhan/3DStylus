#include <opencv2/opencv.hpp>
#include <string_view>

#include "ImageProcessing.hpp"

using namespace cv;

int main(int argc, char* argv[]) {
  const std::string_view path = "../data/ds536/circle_ccw/";
  const std::string_view extension = ".tif";

  auto i = 0;

  while (true) {
    std::ostringstream filename;
    filename << path << std::setw(6) << std::setfill('0') << i << "_depth"
             << extension;
    Mat frame = cv::imread(filename.str(), cv::IMREAD_GRAYSCALE);

    if (frame.empty()) break;

    // Remove far pixels
    Mat thresholdedImage;
    removeBackgroundDepthPixels(frame, thresholdedImage);

    // Use background subtractor to keep the moving pixels only
    auto bgsubtractor{createBackgroundSubtractorMOG2()};
    bgsubtractor->apply(thresholdedImage, thresholdedImage);

    threshold(thresholdedImage, thresholdedImage, 128, 255, THRESH_BINARY);

    // Remove noise
    applyMorphologicalOperation(thresholdedImage, thresholdedImage,
                                cv::MORPH_OPEN, 5);

    // Apply binary mask on original image
    bitwise_not(frame, frame);

    Mat maskedImage;
    bitwise_and(frame, frame, maskedImage, thresholdedImage);

    // Calculate histogram
    Mat hist;
    getHistogramDepthPixels(maskedImage, hist);

    auto maxVal = 255;
    for (int i = hist.rows - 1; i > 0; --i) {
      if (hist.at<int>(i) > 10) {
        maxVal = i;
        break;
      }
    }

    auto maxFreq = 5;
    auto maxIntensity = 255;
    for (int i = 1; i < hist.rows; i++) {
      if (hist.at<float>(i) >= maxFreq) {
        maxIntensity = i;
        maxFreq = hist.at<float>(i);
        break;
      }
    }

    threshold(maskedImage, maskedImage, maxIntensity, 255, THRESH_BINARY);

    // Find and draw centroid
    cv::Point centroid = getCentroid(maskedImage, false);
    drawCentroid(frame, centroid);

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