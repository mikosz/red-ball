#ifndef RED_BALL_GRAPHICS_CHECK_RESULT_HPP_
#define RED_BALL_GRAPHICS_CHECK_RESULT_HPP_

#include <string>

#include "red_ball/windows/windows.hpp"

namespace red_ball {
namespace graphics {

template <class ExceptionType>
void checkResult(HRESULT result, const std::string& errorMessage) {
    if (FAILED(result)) {
        throw ExceptionType(result, errorMessage);
    }
}

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_CHECK_RESULT_HPP_ */
