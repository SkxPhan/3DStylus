#ifndef MEDIASTRATEGY_HPP
#define MEDIASTRATEGY_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

class MediaStrategy {
 public:
  virtual void play(const std::string &path) = 0;
  virtual ~MediaStrategy() = default;
};

#endif  // MEDIASTRATEGY_HPP