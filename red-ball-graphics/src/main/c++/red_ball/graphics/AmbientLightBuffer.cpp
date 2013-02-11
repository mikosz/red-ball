#include "AmbientLightBuffer.hpp"

using namespace red_ball::graphics;

AmbientLightBuffer::AmbientLightBuffer(
        ID3D11Device* device,
        const D3DXVECTOR4& ambientColour
        ) :
        ConstantBuffer(device, sizeof(D3DXVECTOR4)),
        ambientColour_(ambientColour)
{
}

void AmbientLightBuffer::bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const {
    bindData(
            deviceContext,
            bufferNumber,
            reinterpret_cast<const boost::uint8_t*>(&ambientColour_),
            PIXEL_SHADER
            );
}
