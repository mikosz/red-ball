#include "BasicModel.hpp"

#include <vector>
#include <stdexcept>
#include <cassert>

#include <D3DX10math.h>

#include "exceptions.hpp"
#include "Vertex.hpp"

using namespace red_ball::graphics;

BasicModel::BasicModel(ID3D11Device* device, const ModelLoader& modelLoader) :
    texture_(device, "red-ball-graphics/src/main/resources/bitch-please.dds"),
    samplerState_(device),
    vertexCount_(modelLoader.indices()->size())
{
    assert(vertexCount_ > 0);

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = modelLoader.vertices()->size() * sizeof(Vertex);
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = &modelLoader.vertices()->front();

    check(
            device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer_.get()),
            "CreateBuffer failed for vertex buffer"
            );

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = modelLoader.indices()->size() * sizeof(unsigned long);
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA indexBufferData;
    ZeroMemory(&indexBufferData, sizeof(indexBufferData));
    indexBufferData.pSysMem = &modelLoader.indices()->front();

    check(
            device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer_.get()),
            "CreateBuffer failed for index buffer"
            );
}

void BasicModel::render(ID3D11DeviceContext* context) const {
    samplerState_.bind(context);
    texture_.bind(context);

    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    context->IASetVertexBuffers(0, 1, &vertexBuffer_.get(), &stride, &offset);
    context->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(vertexCount_, 0, 0);
}

size_t BasicModel::vertexCount() const {
    return vertexCount_;
}
