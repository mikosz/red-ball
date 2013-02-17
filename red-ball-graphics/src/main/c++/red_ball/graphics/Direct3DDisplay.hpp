#ifndef RED_BALL_GRAPHICS_DIRECT3DDISPLAY_HPP_
#define RED_BALL_GRAPHICS_DIRECT3DDISPLAY_HPP_

#include <stdexcept>

#include <boost/scoped_ptr.hpp>

#include <d3d11.h>
#include <d3dx10math.h>

#include "red_ball/utils/COMWrapper.hpp"
#include "red_ball/windows/windows.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "MatrixBuffer.hpp"
#include "AmbientLightBuffer.hpp"
#include "DirectionalLightBuffer.hpp"
#include "SpecularLightBuffer.hpp"
#include "CameraBuffer.hpp"
#include "Camera.hpp"
#include "RenderingQueue.hpp"
#include "GraphicsContext.hpp"

namespace red_ball {
namespace graphics {

class Direct3DDisplay {
public:

    Direct3DDisplay(GraphicsContext* graphicsContext);

    void render(GraphicsContext* graphicsContext);

private:

    utils::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

    utils::COMWrapper<ID3D11Texture2D> depthBuffer_;

    utils::COMWrapper<ID3D11DepthStencilState> depthStencilState_;

    utils::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

    utils::COMWrapper<ID3D11RasterizerState> rasteriserState_;

    boost::scoped_ptr<Shader> shader_;

    boost::scoped_ptr<MatrixBuffer> worldMatrixBuffer_;

    boost::scoped_ptr<MatrixBuffer> perspectiveProjectionMatrixBuffer_;

    boost::scoped_ptr<MatrixBuffer> orthographicProjectionMatrixBuffer_;

    boost::scoped_ptr<MatrixBuffer> viewMatrixBuffer_;

    boost::scoped_ptr<AmbientLightBuffer> ambientLightBuffer_;

    boost::scoped_ptr<DirectionalLightBuffer> directionalLightBuffer_;

    boost::scoped_ptr<SpecularLightBuffer> specularLightBuffer_;

    boost::scoped_ptr<CameraBuffer> cameraBuffer_;

    Camera camera_;

};

class DisplayCreationError : public DirectXException {
public:

    DisplayCreationError(HRESULT result, const std::string& message);

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_DIRECT3DDISPLAY_HPP_ */
