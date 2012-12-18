#include "DirectionalLightBuffer.hpp"

using namespace red_ball::graphics;

DirectionalLightBuffer::DirectionalLightBuffer(ID3D11Device* device) :
    ConstantBuffer(device, sizeof(Data)) {
}

void DirectionalLightBuffer::bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const {
    Data data;
    data.diffuseColour = D3DXVECTOR4(0.6f, 0.1f, 0.1f, 1.0f);
    data.lightDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    bindData(deviceContext, bufferNumber, reinterpret_cast<boost::uint8_t*>(&data), PIXEL_SHADER);
}
