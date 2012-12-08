#include "ConstantBuffer.hpp"

#include <algorithm>

#include "exceptions.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void setupBuffer(utils::COMWrapper<ID3D11Buffer>* matrixBuffer, ID3D11Device* device, size_t bufferSize) {
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = bufferSize;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    check(device->CreateBuffer(&bufferDesc, 0, &matrixBuffer->get()), "CreateBuffer failed");
}

}  // anonymous namespace

ConstantBuffer::ConstantBuffer(ID3D11Device* device, size_t bufferSize) :
    bufferSize_(bufferSize) {
    setupBuffer(&buffer_, device, bufferSize);
}

void ConstantBuffer::bindData(
        ID3D11DeviceContext* deviceContext,
        size_t bufferNumber,
        const boost::uint8_t* data
        ) const
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    check(
            deviceContext->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource),
            "Resource mapping failed"
            );
    std::copy(data, data + bufferSize_, reinterpret_cast<boost::uint8_t*> (mappedResource.pData));
    deviceContext->Unmap(buffer_, 0);
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &buffer_.get());
}
