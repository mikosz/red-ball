#include "MatrixBuffer.hpp"

#include <boost/cstdint.hpp>

using namespace red_ball::graphics;

MatrixBuffer::MatrixBuffer(ID3D11Device* device) :
        ConstantBuffer(device, sizeof(Data)) {
}

void MatrixBuffer::bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const {
    Data transposed;
    D3DXMatrixTranspose(&transposed.world, &data_.world);
    D3DXMatrixTranspose(&transposed.view, &data_.view);
    D3DXMatrixTranspose(&transposed.projection, &data_.projection);
    bindData(deviceContext, bufferNumber, reinterpret_cast<const boost::uint8_t*> (&transposed));
}
