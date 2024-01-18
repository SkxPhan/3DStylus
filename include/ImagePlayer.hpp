#ifndef IMAGEPLAYER_HPP
#define IMAGEPLAYER_HPP

#include "MediaStrategy.hpp"

class ImagePlayer : public MediaStrategy {
 public:
  ImagePlayer();
  void play(const std::string &path) override;
};

#endif  // IMAGEPLAYER_HPP
