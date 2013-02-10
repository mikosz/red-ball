#ifndef RED_BALL_UTILS_ENDIAN_HPP_
#define RED_BALL_UTILS_ENDIAN_HPP_

#include <boost/cstdint.hpp>

namespace red_ball {
namespace utils {
namespace detail {

template<size_t SIZE>
void swapEndian(boost::uint8_t* target, const boost::uint8_t* source);

// Specialization for 2-byte types.
template<>
inline void swapEndian<2>(boost::uint8_t* target, const boost::uint8_t* source) {
    boost::uint16_t sourceValue = *reinterpret_cast<const boost::uint16_t*>(source);
    boost::uint16_t& targetValue = *reinterpret_cast<boost::uint16_t*>(target);
    targetValue = (sourceValue >> 8) | (sourceValue << 8);
}

// Specialization for 4-byte types.
template<>
inline void swapEndian<4>(boost::uint8_t* target, const boost::uint8_t* source) {
    boost::uint32_t sourceValue = *reinterpret_cast<const boost::uint32_t*>(source);
    boost::uint32_t& targetValue = *reinterpret_cast<boost::uint32_t*>(target);
    targetValue =
            (sourceValue >> 24) |
            ((sourceValue & 0x00ff0000) >> 8) |
            ((sourceValue & 0x0000ff00) << 8) |
            (sourceValue << 24)
            ;
}

} // detail

template<class T>
T swapEndian(T source) {
    T target;
    detail::swapEndian<sizeof(T)>(
            reinterpret_cast<boost::uint8_t*>(&source),
            reinterpret_cast<const boost::uint8_t*>(&target)
            );
    return target;
}

inline bool hostIsBigEndian() {
    union {
        boost::uint16_t full;
        boost::uint8_t parts[2];
    } one;
    one.full = 1;
    return one.parts[1];
}

const bool HOST_IS_BIG_ENDIAN = hostIsBigEndian();

} // namespace utils
} // namespace red_ball

#endif /* RED_BALL_UTILS_ENDIAN_HPP_ */
