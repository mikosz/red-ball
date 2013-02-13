#include "CameraBuffer.hpp"

using namespace red_ball::graphics;

CameraBuffer::CameraBuffer(ID3D11Device* device, const D3DXVECTOR3& cameraPosition) :
        ConstantBuffer(device, sizeof(Data)),
        data_(cameraPosition)
{
}

void CameraBuffer::bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const {
    bindData(deviceContext, bufferNumber, reinterpret_cast<const boost::uint8_t*>(&data_), VERTEX_SHADER);
}

CameraBuffer::Data::Data(const D3DXVECTOR3& cameraPosition_) :
    cameraPosition(cameraPosition_)
{
}
