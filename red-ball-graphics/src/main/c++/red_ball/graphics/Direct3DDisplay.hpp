#ifndef RED_BALL_GRAPHICS_DIRECT3DDISPLAY_HPP_
#define RED_BALL_GRAPHICS_DIRECT3DDISPLAY_HPP_

#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <boost/scoped_ptr.hpp>

#include <d3d11.h>
#include <d3dx10math.h>

#include "red_ball/utils/COMWrapper.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "MatrixBuffer.hpp"
#include "AmbientLightBuffer.hpp"
#include "DirectionalLightBuffer.hpp"
#include "Camera.hpp"

namespace red_ball {
namespace graphics {

class Direct3DDisplay {
public:

    Direct3DDisplay(HWND hWnd);

    void render();

private:

    utils::COMWrapper<ID3D11Device> device_;

    utils::COMWrapper<ID3D11DeviceContext> deviceContext_;

    utils::COMWrapper<IDXGISwapChain> swapChain_;

    utils::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

    utils::COMWrapper<ID3D11Texture2D> depthBuffer_;

    utils::COMWrapper<ID3D11DepthStencilState> depthStencilState_;

    utils::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

    utils::COMWrapper<ID3D11RasterizerState> rasteriserState_;

    D3DXMATRIX perspectiveProjectionMatrix_;

    D3DXMATRIX orthographicProjectionMatrix_;

    boost::scoped_ptr<Shader> shader_;

    boost::scoped_ptr<Model> model_;

    boost::scoped_ptr<MatrixBuffer> matrixBuffer_;

    boost::scoped_ptr<AmbientLightBuffer> ambientLightBuffer_;

    boost::scoped_ptr<DirectionalLightBuffer> directionalLightBuffer_;

    Camera camera_;

};

class DisplayCreationError : public std::runtime_error {
public:

    DisplayCreationError(const std::string& message) :
        std::runtime_error("Failed to create a direct3d display. Error: "/* + message*/) {
    }

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_DIRECT3DDISPLAY_HPP_ */
