#include <opencv2/opencv.hpp>

void removeBackgroundDepthPixels(const cv::Mat& src, cv::Mat& dst);

void getHistogramDepthPixels(const cv::Mat& src, cv::Mat& hist,
                             const cv::Mat& binaryMask = cv::Mat());

int findIntensityWithHighestFrequency(const cv::Mat& hist);

void applyMorphologicalOperation(const cv::Mat& src, cv::Mat& dst,
                                 cv::MorphTypes op, int kernelSize);

cv::Point getCentroid(const cv::Mat& src, bool binaryImage = true);

void drawCentroid(cv::Mat& src, cv::Point centroid);
