#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[]) {
  constexpr auto bgthresholdValue = 3;

  std::string path = "../data/ds536/circle_ccw/";
  auto i = 0;

  while (true) {
    std::ostringstream filename;
    filename << path << std::setw(6) << std::setfill('0') << i << "_depth.tif";
    Mat frame = cv::imread(filename.str(), cv::IMREAD_GRAYSCALE);

    if (frame.empty()) break;

    // Remove far pixels
    Mat thresholdedImage;
    threshold(frame, thresholdedImage, bgthresholdValue, 255, THRESH_BINARY);

    // Use background subtractor to keep the moving pixels only
    auto bgsubtractor{createBackgroundSubtractorMOG2()};
    bgsubtractor->apply(thresholdedImage, thresholdedImage);

    threshold(thresholdedImage, thresholdedImage, 128, 255, THRESH_BINARY);

    // Remove noise
    Mat kernel{getStructuringElement(MORPH_RECT, Size(5, 5))};
    morphologyEx(thresholdedImage, thresholdedImage, MORPH_OPEN, kernel);

    // Apply binary mask on original image
    bitwise_not(frame, frame);

    Mat maskedImage;
    bitwise_and(frame, frame, maskedImage, thresholdedImage);

    // Calculate histogram
    int histSize = 256;  // Number of bins
    float range[] = {0, 255};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&maskedImage, 1, nullptr, Mat(), hist, 1, &histSize,
                 &histRange);

    auto maxVal = 255;
    for (int i = hist.rows - 1; i > 0; --i) {
      if (hist.at<int>(i) > 10) {
        maxVal = i;
        break;
      }
    }

    // Print intensity and pixel count
    auto maxFreq = 5;
    auto maxIntensity = 255;
    for (int i = 1; i < histSize; i++) {
      if (hist.at<float>(i) >= maxFreq) {
        maxIntensity = i;
        maxFreq = hist.at<float>(i);
        break;
      }
    }

    threshold(maskedImage, maskedImage, maxIntensity, 255, THRESH_BINARY);

    // Detect center of mass
    Moments moments = cv::moments(maskedImage, true);
    double cx = moments.m10 / moments.m00;
    double cy = moments.m01 / moments.m00;
    cv::circle(frame, cv::Point(cx, cy), 3, cv::Scalar(0), -1);

    Mat combinedFrame;
    hconcat(frame, thresholdedImage, combinedFrame);
    hconcat(combinedFrame, maskedImage, combinedFrame);
    resize(combinedFrame, combinedFrame, Size(3 * 320, 240));
    imshow("Depth video", combinedFrame);

    if (waitKey(1000 / 30) == 27) break;

    i++;
  }

  destroyAllWindows();

  return 0;
}