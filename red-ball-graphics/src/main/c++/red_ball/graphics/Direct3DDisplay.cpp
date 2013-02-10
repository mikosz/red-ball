#include "Direct3DDisplay.hpp"

#include <vector>
#include <utility>
#include <string>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <dxgi.h>
#include <d3dcommon.h>

#include "red_ball/utils/COMWrapper.hpp"

// XXX: TEST
#include "BasicModel.hpp"
#include "ObjFileLoader.hpp"
#include "BasicShader.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void check(HRESULT result, const std::string& errorMessage) {
    if (FAILED(result)) {
        throw DisplayCreationError(errorMessage);
    }
}

std::pair<int, int> screenResolution() {
    return std::make_pair(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

std::pair<unsigned int, unsigned int> refreshRate(IDXGIAdapter& dxgiAdapter, const std::pair<int, int>& screenResolution) {
    utils::COMWrapper<IDXGIOutput> dxgiOutput;
    check(dxgiAdapter.EnumOutputs(0, &dxgiOutput.get()), "failed to enumerate outputs");

    unsigned int modesCount;
    check(dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modesCount, 0),
            "failed to get number of display modes");

    std::vector<DXGI_MODE_DESC> modes(modesCount);
    check(dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modesCount, &modes.front()),
            "failed to retrieve display modes");

    std::pair<unsigned int, unsigned int> rate(0, 0);
    float comparableRate = 0.0f;
    for (size_t i = 0; i < modes.size(); ++i) {
        if (modes[i].Width == screenResolution.first && modes[i].Height == screenResolution.second) {
            float thisRate = static_cast<float> (modes[i].RefreshRate.Numerator) / modes[i].RefreshRate.Denominator;
            if (comparableRate < thisRate) {
                rate.first = modes[i].RefreshRate.Numerator;
                rate.second = modes[i].RefreshRate.Denominator;
                comparableRate = thisRate;
            }
        }
    }

    return rate;
}

struct DisplayInfo {

    struct VideoCardInfo {

        size_t memory;

        std::wstring name;

    };

    std::pair<int, int> resolution;

    std::pair<unsigned int, unsigned int> refreshRate;

    VideoCardInfo videoCard;

};

DisplayInfo::VideoCardInfo videoCardInfo(IDXGIAdapter& dxgiAdapter) {
    DXGI_ADAPTER_DESC adapterDesc;
    check(dxgiAdapter.GetDesc(&adapterDesc), "failed to get adapter description");

    DisplayInfo::VideoCardInfo result;
    result.memory = adapterDesc.DedicatedVideoMemory;
    result.name = adapterDesc.Description;

    return result;
}

void displayInfo(DisplayInfo* info) {
    utils::COMWrapper<IDXGIFactory> dxgiFactory;
    check(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**> (&dxgiFactory.get())), "failed to create a dxgi factory");

    utils::COMWrapper<IDXGIAdapter> dxgiAdapter;
    check(dxgiFactory->EnumAdapters(0, &dxgiAdapter.get()), "failed to enumerate adapters");

    info->resolution = screenResolution();
    info->refreshRate = refreshRate(*dxgiAdapter, info->resolution);
    info->videoCard = videoCardInfo(*dxgiAdapter);
}

void setupDevice(
        utils::COMWrapper<ID3D11Device>* device,
        utils::COMWrapper<ID3D11DeviceContext>* deviceContext,
        utils::COMWrapper<IDXGISwapChain>* swapChain,
        D3D_FEATURE_LEVEL* featureLevel,
        HWND hWnd,
        const DisplayInfo& info) {
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.Windowed = true;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = info.resolution.first;
    swapChainDesc.BufferDesc.Height = info.resolution.second;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = info.refreshRate.first;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = info.refreshRate.second;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    static const D3D_FEATURE_LEVEL acceptedFeatureLevels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1,
    };
    static const size_t TOTAL_FEATURE_LEVELS = sizeof(acceptedFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

    check(
            D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, acceptedFeatureLevels,
                    TOTAL_FEATURE_LEVELS, D3D11_SDK_VERSION,
                    &swapChainDesc, &swapChain->get(), &device->get(), featureLevel, &deviceContext->get()),
            "failed to create device and swap chain");
}

void setupRenderTargetView(
        utils::COMWrapper<ID3D11RenderTargetView>* renderTargetView,
        IDXGISwapChain& swapChain,
        ID3D11Device& device
        ) {
    utils::COMWrapper<ID3D11Texture2D> buffer;
    check(swapChain.GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**> (&buffer.get())), "failed to retrieve back buffer");
    check(device.CreateRenderTargetView(buffer, 0, &renderTargetView->get()), "failed to create a render target view");
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

Direct3DDisplay::Direct3DDisplay(HWND hWnd) :
    camera_(
            D3DXVECTOR3(0.0f, 0.0f, -10.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f)
            )
{
    DisplayInfo info;
    displayInfo(&info);

    D3D_FEATURE_LEVEL featureLevel;
    setupDevice(&device_, &deviceContext_, &swapChain_, &featureLevel, hWnd, info);
    setupRenderTargetView(&renderTargetView_, *swapChain_, *device_);
    setupDepthBuffer(&depthBuffer_, *device_, info.resolution.first, info.resolution.second);
    setupDepthStencilState(&depthStencilState_, *device_, *deviceContext_);
    setupDepthStencilView(&depthStencilView_, depthBuffer_, device_);
    setupRasteriser(&rasteriserState_, *device_, *deviceContext_);
    setupViewport(deviceContext_, info.resolution.first, info.resolution.second);
    setupOrthographicProjectionMatrix(&orthographicProjectionMatrix_, info.resolution.first, info.resolution.second);
    setupPerspectiveProjectionMatrix(&perspectiveProjectionMatrix_, info.resolution.first, info.resolution.second);

    deviceContext_->OMSetRenderTargets(1, &renderTargetView_.get(), depthStencilView_);

    shader_.reset(
            new BasicShader(
                    device_,
                    "red-ball-graphics/src/main/hlsl/simple.vs",
                    "main",
                    "red-ball-graphics/src/main/hlsl/simple.ps",
                    "main"
                    )
    );

    ObjFileLoader loader("red-ball-graphics/src/main/resources/cube.objfile");
    model_.reset(new BasicModel(device_, loader));

    matrixBuffer_.reset(new MatrixBuffer(device_));
    D3DXMatrixIdentity(&matrixBuffer_->world());

    directionalLightBuffer_.reset(new DirectionalLightBuffer(device_));
}

void Direct3DDisplay::render() {
    static const float BLACK[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext_->ClearRenderTargetView(renderTargetView_, BLACK);
    deviceContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

    matrixBuffer_->projection() = perspectiveProjectionMatrix_;
    camera_.viewMatrix(&matrixBuffer_->view());
    matrixBuffer_->bind(deviceContext_, 0);
    directionalLightBuffer_->bind(deviceContext_, 0);
    shader_->bind(deviceContext_);
    model_->render(deviceContext_);

    swapChain_->Present(1, 0);
}
