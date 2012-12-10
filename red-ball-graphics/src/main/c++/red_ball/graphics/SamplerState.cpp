#include "SamplerState.hpp"

#include "exceptions.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void setupSamplerState(utils::COMWrapper<ID3D11SamplerState>* samplerState, ID3D11Device* device) {
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    check(
            device->CreateSamplerState(&samplerDesc, &samplerState->get()),
            "CreateSamplerState failed"
            );
}

} // anonymous namespace

SamplerState::SamplerState(ID3D11Device* device) {
    setupSamplerState(&samplerState_, device);
}

void SamplerState::bind(ID3D11DeviceContext* deviceContext) const {
    deviceContext->PSSetSamplers(0, 1, &samplerState_.get());
}
