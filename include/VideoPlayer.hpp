#ifndef VIDEOPLAYER_HPP
#define VIDEOPLAYER_HPP

#include "MediaStrategy.hpp"

class VideoPlayer : public MediaStrategy
{
public:
    VideoPlayer(int frameRate);
    void play(const std::string &path) override;

private:
    int frameRate;
};

#endif // VIDEOPLAYER_HPP
