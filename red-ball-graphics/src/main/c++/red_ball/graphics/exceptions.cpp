#include "exceptions.hpp"

#include <DxErr.h>

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

std::string directXExceptionMessage(HRESULT hResult, const std::string& message) {
    std::ostringstream oss;
    if (!message.empty()) {
        oss << message << '\n';
    }
    oss << "DirectX error: " << DXGetErrorStringA(hResult) << ". Description: "
            << DXGetErrorDescriptionA(hResult);
    return oss.str();
}

}  // anonymous namespace

DirectXException::DirectXException(HRESULT hResult, const std::string& message) :
    core::RuntimeError(directXExceptionMessage(hResult, message)) {
}

void red_ball::graphics::check(HRESULT hResult, const std::string& message) {
    if (FAILED(hResult)) {
        throw DirectXException(hResult, message);
    }
}
