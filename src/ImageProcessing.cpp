#include "ImageProcessing.hpp"

void applyMorphologicalOperation(const cv::Mat& src, cv::Mat& dst,
                                 cv::MorphTypes op, int kernelSize) {
  if (kernelSize % 2 == 0) {
    throw std::invalid_argument("Kernel size must be odd.");
  }
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                             cv::Size(kernelSize, kernelSize));
  cv::morphologyEx(src, dst, op, kernel);
}

void removeBackgroundDepthPixels(const cv::Mat& src, cv::Mat& dst) {
  cv::Mat hist;
  getHistogramDepthPixels(src, hist);

  cv::threshold(src, dst, 3, 255, cv::THRESH_BINARY);
  // cv::adaptiveThreshold(src, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
  //                       cv::THRESH_BINARY_INV, 11, 2);
  // compute histogram
  // get only the pixels with highest frequency that are not 255
}

void getHistogramDepthPixels(const cv::Mat& src, cv::Mat& hist,
                             const cv::Mat& binaryMask) {
  auto histSize = 256;
  float range[] = {0, 255};
  const float* histRange = {range};
  cv::calcHist(&src, 1, 0, binaryMask, hist, 1, &histSize, &histRange);
}

int findIntensityWithHighestFrequency(const cv::Mat& hist) {
  int maxFreq = 5;
  int maxIntensity = 255;
  // Iterate from 1 to 254 to exclude endpoints
  for (int i = 1; i < hist.rows - 1; i++) {
    if (hist.at<int>(i) >= maxFreq) {
      maxIntensity = i;
      maxFreq = hist.at<int>(i);
      break;
    }
  }
  return maxIntensity;
}

cv::Point getCentroid(const cv::Mat& src, bool binaryImage) {
  cv::Moments moments = cv::moments(src, binaryImage);
  return cv::Point(moments.m10 / moments.m00, moments.m01 / moments.m00);
}

void drawCentroid(cv::Mat& src, cv::Point centroid) {
  cv::circle(src, centroid, 3, cv::Scalar(0), -1);
}
