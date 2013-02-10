#ifndef RED_BALL_UTILS_POINTEE_HPP_
#define RED_BALL_UTILS_POINTEE_HPP_

#include "red_ball/core/exceptions.hpp"

namespace red_ball {
namespace utils {

class NullPointerException : public core::RuntimeError {
public:

    NullPointerException() :
        core::RuntimeError("Attempted to dereference a null pointer") {
    }

    ~NullPointerException() throw () {
    }

};

template <class T>
T& pointee(T* ptr) {
    if (ptr) {
        return *ptr;
    } else {
        throw NullPointerException();
    }
}

}  // namespace utils
}  // namespace red_ball

#endif /* RED_BALL_UTILS_POINTEE_HPP_ */
