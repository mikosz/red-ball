#ifndef RED_BALL_GRAPHICS_GRAPHICSCONTEXT_HPP_
#define RED_BALL_GRAPHICS_GRAPHICSCONTEXT_HPP_

#include <boost/shared_ptr.hpp>

#include <d3d11.h>

#include "red_ball/windows/windows.hpp"
#include "red_ball/utils/COMWrapper.hpp"
#include "DisplayInfo.hpp"
#include "RenderingQueue.hpp"
#include "exceptions.hpp"

namespace red_ball {
namespace graphics {

class GraphicsContext {
public:

    GraphicsContext(HWND hWnd);

    const DisplayInfo& displayInfo() const {
        return displayInfo_;
    }

    ID3D11Device& device() {
        return *device_;
    }

    ID3D11DeviceContext& deviceContext() {
        return *deviceContext_;
    }

    IDXGISwapChain& swapChain() {
        return *swapChain_;
    }

    boost::shared_ptr<graphics::RenderingQueue> renderingQueue() {
        return renderingQueue_;
    }

private:

    DisplayInfo displayInfo_;

    utils::COMWrapper<ID3D11Device> device_;

    utils::COMWrapper<ID3D11DeviceContext> deviceContext_;

    utils::COMWrapper<IDXGISwapChain> swapChain_;

    D3D_FEATURE_LEVEL featureLevel_;

    boost::shared_ptr<graphics::RenderingQueue> renderingQueue_;

};

class GraphicsContextSetupError : public DirectXException {
public:

    GraphicsContextSetupError(HRESULT result, const std::string& message);

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_GRAPHICSCONTEXT_HPP_ */
