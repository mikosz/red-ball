#include "GraphicsContext.hpp"

#include <dxgi.h>

#include "check-result.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void setupDevice(
        utils::COMWrapper<ID3D11Device>* device,
        utils::COMWrapper<ID3D11DeviceContext>* deviceContext,
        utils::COMWrapper<IDXGISwapChain>* swapChain,
        D3D_FEATURE_LEVEL* featureLevel,
        HWND hWnd,
        const DisplayInfo& displayInfo) {
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.Windowed = true;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = displayInfo.resolution().first;
    swapChainDesc.BufferDesc.Height = displayInfo.resolution().second;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = displayInfo.refreshRate().first;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = displayInfo.refreshRate().second;
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

    checkResult<GraphicsContextSetupError>(
            D3D11CreateDeviceAndSwapChain(
                    0,
                    D3D_DRIVER_TYPE_HARDWARE,
                    0,
                    0,
                    acceptedFeatureLevels,
                    TOTAL_FEATURE_LEVELS,
                    D3D11_SDK_VERSION,
                    &swapChainDesc,
                    &swapChain->get(),
                    &device->get(),
                    featureLevel,
                    &deviceContext->get()
                    ),
            "failed to create device and swap chain"
            );
}

} // anonymous namespace

GraphicsContext::GraphicsContext(HWND hWnd) :
    renderingQueue_(new RenderingQueue)
{
    setupDevice(&device_, &deviceContext_, &swapChain_, &featureLevel_, hWnd, displayInfo_);
}

GraphicsContextSetupError::GraphicsContextSetupError(HRESULT result, const std::string& message) :
    DirectXException(result, message)
{
}
