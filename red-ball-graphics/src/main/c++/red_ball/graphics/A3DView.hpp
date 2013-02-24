#ifndef RED_BALL_GRAPHICS_A3DVIEW_HPP_
#define RED_BALL_GRAPHICS_A3DVIEW_HPP_

#include <stdexcept>

#include <boost/scoped_ptr.hpp>

#include <d3d11.h>
#include <d3dx10math.h>

#include "red_ball/utils/COMWrapper.hpp"
#include "red_ball/windows/windows.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "RenderingQueue.hpp"
#include "GraphicsContext.hpp"

namespace red_ball {
namespace graphics {

class A3DView {
public:

    A3DView(GraphicsContext* graphicsContext);

    void render(GraphicsContext* graphicsContext);

private:

    utils::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

    utils::COMWrapper<ID3D11Texture2D> depthBuffer_;

    utils::COMWrapper<ID3D11DepthStencilState> depthStencilState_;

    utils::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

    utils::COMWrapper<ID3D11RasterizerState> rasteriserState_;

    boost::scoped_ptr<Shader> shader_;

};

class DisplayCreationError : public DirectXException {
public:

    DisplayCreationError(HRESULT result, const std::string& message);

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_A3DVIEW_HPP_ */
