#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[]) {
  constexpr auto thresholdValue = 3;

  std::string path{"../data/ds536/rectangle_cw"};
  VideoCapture cap(path + "/%06d_depth.tif", CAP_ANY);

  while (true) {
    Mat frame;
    cap >> frame;
    if (frame.empty()) break;

    // Remove far pixels
    Mat thresholdedImage;
    threshold(frame, thresholdedImage, thresholdValue, 255, THRESH_BINARY);

    // Use background subtractor to keep the moving pixels only
    auto bgsubtractor{createBackgroundSubtractorMOG2()};
    bgsubtractor->apply(thresholdedImage, thresholdedImage);

    // Remove noise
    Mat kernel{getStructuringElement(MORPH_RECT, Size(5, 5))};
    morphologyEx(thresholdedImage, thresholdedImage, MORPH_OPEN, kernel);

    // To get a binary mask
    threshold(thresholdedImage, thresholdedImage, 128, 255, THRESH_BINARY);

    // Detect center of mass
    Moments moments = cv::moments(thresholdedImage, true);
    double cx = moments.m10 / moments.m00;
    double cy = moments.m01 / moments.m00;
    cv::circle(frame, cv::Point(cx, cy), 3, cv::Scalar(0, 0, 255), -1);

    Mat combinedFrame;
    cvtColor(thresholdedImage, thresholdedImage, COLOR_GRAY2BGR);
    hconcat(frame, thresholdedImage, combinedFrame);
    resize(combinedFrame, combinedFrame, Size(2 * 320, 240));
    imshow("Depth video", combinedFrame);

    if (waitKey(1000 / 30) == 27) break;
  }

  cap.release();

  destroyAllWindows();

  return 0;
}
