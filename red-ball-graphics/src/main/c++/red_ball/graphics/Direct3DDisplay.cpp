#include "Direct3DDisplay.hpp"

#include <vector>
#include <utility>
#include <string>
#include <algorithm>

#include <d3dcommon.h>

#include "red_ball/utils/COMWrapper.hpp"
#include "red_ball/utils/pointee.hpp"
#include "check-result.hpp"
#include "BasicShader.hpp"
#include "RenderingQueue.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void setupRenderTargetView(
        utils::COMWrapper<ID3D11RenderTargetView>* renderTargetView,
        IDXGISwapChain& swapChain,
        ID3D11Device& device
        ) {
    utils::COMWrapper<ID3D11Texture2D> buffer;
    checkResult<DisplayCreationError>(
            swapChain.GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&buffer.get())),
            "failed to retrieve back buffer");
    checkResult<DisplayCreationError>(device.CreateRenderTargetView(buffer, 0, &renderTargetView->get()),
            "failed to create a render target view");
}

void setupDepthBuffer(
        utils::COMWrapper<ID3D11Texture2D>* depthBuffer,
        ID3D11Device& device,
        size_t screenWidth,
        size_t screenHeight
        ) {
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width = screenWidth;
    depthBufferDesc.Height = screenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    check(device.CreateTexture2D(&depthBufferDesc, 0, &depthBuffer->get()), "failed to create depth buffer");
}

void setupDepthStencilState(
        utils::COMWrapper<ID3D11DepthStencilState>* depthStencilState,
        ID3D11Device& device,
        ID3D11DeviceContext& deviceContext
        ) {
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xff;
    depthStencilDesc.StencilWriteMask = 0xff;

    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    check(device.CreateDepthStencilState(&depthStencilDesc, &depthStencilState->get()), "failed to create a depth stencil state");

    deviceContext.OMSetDepthStencilState(*depthStencilState, 1);
}

void setupDepthStencilView(
        utils::COMWrapper<ID3D11DepthStencilView>* depthStencilView,
        ID3D11Texture2D* depthBuffer,
        ID3D11Device* device
        ) {
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    check(device->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &depthStencilView->get()), "failed to create a depth stencil view");
}

void setupRasteriser(
        utils::COMWrapper<ID3D11RasterizerState>* rasteriserState,
        ID3D11Device& device,
        ID3D11DeviceContext& deviceContext
        ) {
    D3D11_RASTERIZER_DESC rasteriserDesc;
    ZeroMemory(&rasteriserDesc, sizeof(rasteriserDesc));
    rasteriserDesc.CullMode = D3D11_CULL_BACK;
    rasteriserDesc.DepthBiasClamp = 0.0f;
    rasteriserDesc.DepthClipEnable = true;
    rasteriserDesc.FillMode = D3D11_FILL_SOLID;
    rasteriserDesc.SlopeScaledDepthBias = 0.0f;

    check(device.CreateRasterizerState(&rasteriserDesc, &rasteriserState->get()), "failed to create a rasteriser state");

    deviceContext.RSSetState(*rasteriserState);
}

void setupViewport(
        ID3D11DeviceContext* deviceContext,
        size_t screenWidth,
        size_t screenHeight
        ) {
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(viewport));
    viewport.Width = screenWidth;
    viewport.Height = screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    deviceContext->RSSetViewports(1, &viewport);
}

void setupPerspectiveProjectionMatrix(
        D3DXMATRIX* matrix,
        size_t screenWidth,
        size_t screenHeight
        ) {
    float aspectRatio = static_cast<float> (screenWidth) / screenHeight;
    D3DXMatrixPerspectiveFovLH(matrix, D3DX_PI / 4.0f, aspectRatio, 1.0f, 100.0f);
}

void setupOrthographicProjectionMatrix(
        D3DXMATRIX* matrix,
        size_t screenWidth,
        size_t screenHeight
        ) {
    D3DXMatrixOrthoLH(matrix, screenWidth, screenHeight, 1.0f, 100.0f);
}

}  // anonymous namespace

Direct3DDisplay::Direct3DDisplay(GraphicsContext* graphicsContextPtr) :
    camera_(
            D3DXVECTOR3(0.0f, 0.0f, -10.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f)
            )
{
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    setupRenderTargetView(&renderTargetView_, graphicsContext.swapChain(), graphicsContext.device());
    setupDepthBuffer(&depthBuffer_, graphicsContext.device(), graphicsContext.displayInfo().resolution().first,
            graphicsContext.displayInfo().resolution().second);
    setupDepthStencilState(&depthStencilState_, graphicsContext.device(), graphicsContext.deviceContext());
    setupDepthStencilView(&depthStencilView_, depthBuffer_, &graphicsContext.device());
    setupRasteriser(&rasteriserState_, graphicsContext.device(), graphicsContext.deviceContext());
    setupViewport(&graphicsContext.deviceContext(), graphicsContext.displayInfo().resolution().first,
            graphicsContext.displayInfo().resolution().second);

    orthographicProjectionMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));
    setupOrthographicProjectionMatrix(
            &orthographicProjectionMatrixBuffer_->matrix(),
            graphicsContext.displayInfo().resolution().first,
            graphicsContext.displayInfo().resolution().second
            );

    perspectiveProjectionMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));
    setupPerspectiveProjectionMatrix(
            &perspectiveProjectionMatrixBuffer_->matrix(),
            graphicsContext.displayInfo().resolution().first,
            graphicsContext.displayInfo().resolution().second
            );

    viewMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));

    worldMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));

    graphicsContext.deviceContext().OMSetRenderTargets(1, &renderTargetView_.get(), depthStencilView_);

    shader_.reset(
            new BasicShader(
                    &graphicsContext.device(),
                    "red-ball-graphics/src/main/hlsl/simple.vs",
                    "main",
                    "red-ball-graphics/src/main/hlsl/simple.ps",
                    "main"
                    )
    );

    ambientLightBuffer_.reset(
            new AmbientLightBuffer(&graphicsContext.device(), D3DXVECTOR4(0.15f, 0.15f, 0.15f, 1.0f)));

    directionalLightBuffer_.reset(
            new DirectionalLightBuffer(&graphicsContext.device(), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f),
                    D3DXVECTOR3(-1.0f, -1.0f, 1.0f)));

    specularLightBuffer_.reset(
            new SpecularLightBuffer(&graphicsContext.device(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 16.0f));

    cameraBuffer_.reset(new CameraBuffer(&graphicsContext.device(), camera_.position()));
}

void Direct3DDisplay::render(GraphicsContext* graphicsContextPtr) {
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    static const float BLACK[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    graphicsContext.deviceContext().ClearRenderTargetView(renderTargetView_, BLACK);
    graphicsContext.deviceContext().ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

    camera_.viewMatrix(&viewMatrixBuffer_->matrix());
    viewMatrixBuffer_->bind(&graphicsContext.deviceContext(), 1);
    perspectiveProjectionMatrixBuffer_->bind(&graphicsContext.deviceContext(), 2);
    cameraBuffer_->cameraPosition() = camera_.position();
    cameraBuffer_->bind(&graphicsContext.deviceContext(), 3);

    ambientLightBuffer_->bind(&graphicsContext.deviceContext(), 0);
    directionalLightBuffer_->bind(&graphicsContext.deviceContext(), 1);
    specularLightBuffer_->bind(&graphicsContext.deviceContext(), 2);

    shader_->bind(&graphicsContext.deviceContext());

    graphicsContext.renderingQueue()->render(&graphicsContext.deviceContext(), worldMatrixBuffer_.get());

    graphicsContext.swapChain().Present(1, 0);
}

DisplayCreationError::DisplayCreationError(HRESULT result, const std::string& message) :
        DirectXException(result, "Failed to create a direct3d display. Error: " + message)
{
}
