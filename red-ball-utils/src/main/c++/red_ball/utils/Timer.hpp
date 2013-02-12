#ifndef RED_BALL_UTILS_TIMER_HPP_
#define RED_BALL_UTILS_TIMER_HPP_

#include <ctime>

namespace red_ball {
namespace utils {

class Timer {
public:

    typedef float Seconds;

    Timer();

    Seconds tick();

    Seconds lastFrameDuration() const {
        return lastFrameDuration_;
    }

private:

    std::clock_t lastTick_;

    Seconds lastFrameDuration_;

};

} // namespace utils
} // namespace red_ball

#endif /* RED_BALL_UTILS_TIMER_HPP_ */
