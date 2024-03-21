#include "ImageProcessing.hpp"

#include <cmath>

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

//   int first_peak_value = 255;
//   for (int i = hist.rows - 1; i > 0; --i) {
//     if (hist.at<float>(i) > 1000) {
//       first_peak_value = i;
//       break;
//     }
//   }

  cv::threshold(src, dst, 251, 255, cv::THRESH_BINARY);
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
  cv::circle(src, centroid, 3, cv::Scalar(255), -1);
}

bool isMoving(const std::vector<std::pair<float, float>>& points,
              int nbrPreviousPoint) {
  if (points.size() > nbrPreviousPoint) {
    std::pair<float, float> lastPoint = points.back();
    std::pair<float, float> previousPoint =
        points[points.size() - nbrPreviousPoint];
    return std::pow(lastPoint.first - previousPoint.first, 2) +
               std::pow(lastPoint.second - previousPoint.second, 2) >
           16;
  }
  return true;
}

void applyHandSegmentation(const cv::Mat& src, cv::Mat& dst) {
  cv::Mat hist;
  getHistogramDepthPixels(src, hist);
  auto maxIntensity = findIntensityWithHighestFrequency(hist);
  threshold(src, dst, maxIntensity, 255, cv::THRESH_BINARY);
  applyMorphologicalOperation(dst, dst, cv::MORPH_CLOSE, 5);
}