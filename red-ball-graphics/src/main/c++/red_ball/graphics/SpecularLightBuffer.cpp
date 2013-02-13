#include "SpecularLightBuffer.hpp"

using namespace red_ball::graphics;

SpecularLightBuffer::SpecularLightBuffer(
        ID3D11Device* device,
        const D3DXVECTOR4& specularColour,
        float specularPower
        ) :
        ConstantBuffer(device, sizeof(Data)),
        data_(specularColour, specularPower)
{
}

void SpecularLightBuffer::bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const {
    bindData(deviceContext, bufferNumber, reinterpret_cast<const boost::uint8_t*>(&data_), PIXEL_SHADER);
}

SpecularLightBuffer::Data::Data(const D3DXVECTOR4& specularColour_, float specularPower_) :
    specularColour(specularColour_),
    specularPower(specularPower_)
{
}
