#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <opencv2/opencv.hpp>

void applyMorphologicalOperation(const cv::Mat& src, cv::Mat& dst,
                                 cv::MorphTypes op, int kernelSize);

void removeBackgroundDepthPixels(const cv::Mat& src, cv::Mat& dst);

void getHistogramDepthPixels(const cv::Mat& src, cv::Mat& hist,
                             const cv::Mat& binaryMask = cv::Mat());

void applyHandSegmentation(const cv::Mat& src, cv::Mat& dst);

int findIntensityWithHighestFrequency(const cv::Mat& hist);

cv::Point getCentroid(const cv::Mat& src, bool binaryImage = true);

void drawCentroid(cv::Mat& src, cv::Point centroid);

bool isMoving(const std::vector<std::pair<float, float>>& points,
              int nbrPreviousPoint = 20);

#endif  // IMAGE_PROCESSING_H