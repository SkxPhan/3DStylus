#ifndef MEDIASTRATEGY_HPP
#define MEDIASTRATEGY_HPP

#include <string>

class MediaStrategy
{
public:
    virtual void play(const std::string &path) = 0;
    virtual ~MediaStrategy() = default;
};

#endif // MEDIASTRATEGY_HPP