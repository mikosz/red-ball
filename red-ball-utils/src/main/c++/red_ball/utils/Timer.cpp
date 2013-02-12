#include "Timer.hpp"

using namespace red_ball::utils;

Timer::Timer() :
    lastTick_(std::clock()),
    lastFrameDuration_(0.0f)
{
}

Timer::Seconds Timer::tick() {
    clock_t now = std::clock();
    lastFrameDuration_ = static_cast<float>(now - lastTick_) / CLOCKS_PER_SEC;
    lastTick_ = now;

    return lastFrameDuration_;
}
