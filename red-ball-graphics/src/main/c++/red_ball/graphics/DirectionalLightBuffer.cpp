#include "DirectionalLightBuffer.hpp"

using namespace red_ball::graphics;

DirectionalLightBuffer::DirectionalLightBuffer(
        ID3D11Device* device,
        const D3DXVECTOR4& diffuseColour,
        const D3DXVECTOR3& lightDirection
        ) :
        ConstantBuffer(device, sizeof(Data)),
        data_(diffuseColour, lightDirection)
{
}

void DirectionalLightBuffer::bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const {
    bindData(deviceContext, bufferNumber, reinterpret_cast<const boost::uint8_t*>(&data_), PIXEL_SHADER);
}

DirectionalLightBuffer::Data::Data(const D3DXVECTOR4& diffuseColour_, const D3DXVECTOR3& lightDirection_) :
        diffuseColour(diffuseColour_),
        lightDirection(lightDirection_)
{
}
