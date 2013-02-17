#include "DisplayInfo.hpp"

#include <vector>

#include <dxgi.h>

#include "red_ball/utils/COMWrapper.hpp"
#include "red_ball/utils/pointee.hpp"
#include "check-result.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void videoCardInfo(DisplayInfo::VideoCardInfo* videoCardInfoPtr, IDXGIAdapter& dxgiAdapter) {
    DisplayInfo::VideoCardInfo& videoCardInfo = utils::pointee(videoCardInfoPtr);

    DXGI_ADAPTER_DESC adapterDesc;
    checkResult<DisplayInfoDiscoveryError>(
            dxgiAdapter.GetDesc(&adapterDesc),
            "failed to get adapter description"
            );

    videoCardInfo.memory = adapterDesc.DedicatedVideoMemory;
    videoCardInfo.name = adapterDesc.Description;
}

DisplayInfo::Resolution screenResolution() {
    return std::make_pair(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

DisplayInfo::RefreshRate refreshRate(
        IDXGIAdapter& dxgiAdapter,
        const std::pair<int, int>& screenResolution
        ) {
    utils::COMWrapper<IDXGIOutput> dxgiOutput;
    check(dxgiAdapter.EnumOutputs(0, &dxgiOutput.get()), "failed to enumerate outputs");

    unsigned int modesCount;
    checkResult<DisplayInfoDiscoveryError>(
            dxgiOutput->GetDisplayModeList(
                    DXGI_FORMAT_R8G8B8A8_UNORM,
                    DXGI_ENUM_MODES_INTERLACED,
                    &modesCount,
                    0
                    ),
            "failed to get number of display modes"
            );

    std::vector<DXGI_MODE_DESC> modes(modesCount);
    checkResult<DisplayInfoDiscoveryError>(
            dxgiOutput->GetDisplayModeList(
                    DXGI_FORMAT_R8G8B8A8_UNORM,
                    DXGI_ENUM_MODES_INTERLACED,
                    &modesCount,
                    &modes.front()
                    ),
            "failed to retrieve display modes"
            );

    DisplayInfo::RefreshRate rate(0, 0);

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

} // anonymous namespace

DisplayInfo::DisplayInfo() {
    utils::COMWrapper<IDXGIFactory> dxgiFactory;
    checkResult<DisplayInfoDiscoveryError>(
            CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory.get())),
            "failed to create a dxgi factory"
            );

    utils::COMWrapper<IDXGIAdapter> dxgiAdapter;
    checkResult<DisplayInfoDiscoveryError>(
            dxgiFactory->EnumAdapters(0, &dxgiAdapter.get()),
            "failed to enumerate adapters"
            );

    resolution_ = screenResolution();
    refreshRate_ = ::refreshRate(*dxgiAdapter, resolution_);
    videoCardInfo(&videoCard_, *dxgiAdapter);
}

DisplayInfoDiscoveryError::DisplayInfoDiscoveryError(HRESULT result, const std::string& message) :
        DirectXException(result, message)
{
}
