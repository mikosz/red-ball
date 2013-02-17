#ifndef RED_BALL_GRAPHICS_EXCEPTIONS_HPP_
#define RED_BALL_GRAPHICS_EXCEPTIONS_HPP_

#include "red_ball/core/exceptions.hpp"
#include "red_ball/windows/windows.hpp"

namespace red_ball {
namespace graphics {

class DirectXException : public core::RuntimeError {
public:

    DirectXException(HRESULT hResult, const std::string& message = std::string());

    ~DirectXException() throw () {
    }

};

void check(HRESULT hResult, const std::string& message = std::string());

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_EXCEPTIONS_HPP_ */
