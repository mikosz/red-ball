#ifndef RED_BALL_UTILS_CLIP_HPP_
#define RED_BALL_UTILS_CLIP_HPP_

#include <algorithm>

namespace red_ball {
namespace utils {

template <class T>
T clip(T value, T lowerBound, T upperBound) {
    return std::min(std::max(value, lowerBound), upperBound);
}

} // namespace utils
} // namespace red_ball

#endif /* RED_BALL_UTILS_CLIP_HPP_ */
