#include "ImageProcessing.hpp"

void detectForegroundHand(const cv::Mat& src, cv::Mat& dst) {
  cv::Mat invertedFrame;
  bitwise_not(src, invertedFrame);

  cv::Mat openedFrame;
  applyMorphologicalOperation(invertedFrame, openedFrame, cv::MORPH_OPEN, 5);

  cv::Mat foregroundMask;
  removeBackgroundDepthPixels(openedFrame, foregroundMask);

  cv::Mat maskedFrame;
  bitwise_and(invertedFrame, invertedFrame, maskedFrame, foregroundMask);

  applyHandSegmentation(maskedFrame, dst);
}

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
  // cv::Mat hist;
  // getHistogramDepthPixels(src, hist);

  // int first_peak_value = 255;
  // for (int i = hist.rows - 1; i > 0; --i) {
  //   if (hist.at<float>(i) > 1000) {
  //     first_peak_value = i;
  //     break;
  //   }
  // }

  cv::threshold(src, dst, 251, 255, cv::THRESH_BINARY);
}

void getHistogramDepthPixels(const cv::Mat& src, cv::Mat& hist,
                             const cv::Mat& binaryMask) {
  auto histSize = 256;
  float range[] = {0, 255};
  const float* histRange = {range};
  cv::calcHist(&src, 1, 0, binaryMask, hist, 1, &histSize, &histRange);
}

void applyHandSegmentation(const cv::Mat& src, cv::Mat& dst) {
  cv::Mat hist;
  getHistogramDepthPixels(src, hist);
  auto maxIntensity = findIntensityWithHighestFrequency(hist);
  threshold(src, dst, maxIntensity, 255, cv::THRESH_BINARY);
  applyMorphologicalOperation(dst, dst, cv::MORPH_CLOSE, 5);
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

void mergeFrames(const std::vector<cv::Mat>& frames, cv::Mat& combinedFrame) {
  if (frames.empty()) {
    throw std::invalid_argument("No frames provided for merging");
  }

  cv::Size firstFrameSize = frames[0].size();
  combinedFrame = frames[0];
  for (size_t i = 1; i < frames.size(); ++i) {
    if (frames[i].size() != firstFrameSize) {
      throw std::invalid_argument("Frames do not have the same dimensions");
    }
    hconcat(combinedFrame, frames[i], combinedFrame);
  }
}