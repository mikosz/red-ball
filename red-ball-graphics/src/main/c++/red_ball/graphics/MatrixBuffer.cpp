#include "MatrixBuffer.hpp"

#include <boost/cstdint.hpp>

using namespace red_ball::graphics;

MatrixBuffer::MatrixBuffer(ID3D11Device* device) :
        ConstantBuffer(device, sizeof(D3DXMATRIX)) {
}

void MatrixBuffer::bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const {
    D3DXMATRIX transposed;
    D3DXMatrixTranspose(&transposed, &matrix_);
    bindData(deviceContext, bufferNumber, reinterpret_cast<const boost::uint8_t*>(&transposed), VERTEX_SHADER);
}
