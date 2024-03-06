#include <opencv2/opencv.hpp>
#include <string_view>

#include "ImageProcessing.hpp"

using namespace cv;

int main(int argc, char* argv[]) {
  const std::string_view path = "../data/ds536/rectangle_cw/";
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

    bitwise_not(thresholdedImage, thresholdedImage);

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

    auto maxIntensity = findIntensityWithHighestFrequency(hist);

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