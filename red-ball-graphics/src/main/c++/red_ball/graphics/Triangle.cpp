#include "Triangle.hpp"

#include <vector>
#include <stdexcept>

#include <D3DX10math.h>

#include "exceptions.hpp"

using namespace red_ball::graphics;

namespace {

struct VertexType {

    D3DXVECTOR3 position;

    D3DXVECTOR2 textureCoordinates;

    VertexType(const D3DXVECTOR3& position_, const D3DXVECTOR2& textureCoordinates_) :
        position(position_),
        textureCoordinates(textureCoordinates_) {
    }

};

} // anonymous namespace

Triangle::Triangle(ID3D11Device* device) :
    texture_(device, "red-ball-graphics/src/main/resources/bitch-please.dds"),
    samplerState_(device)
{
    std::vector<VertexType> vertices;
    vertices.push_back(VertexType(D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));
    vertices.push_back(VertexType(D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.5f, 0.0f)));
    vertices.push_back(VertexType(D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f)));

    std::vector<unsigned long> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    vertexCount_ = indices.size();

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = vertices.size() * sizeof(VertexType);
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = &vertices.front();

    check(
            device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer_.get()),
            "CreateBuffer failed for vertex buffer"
            );

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = indices.size() * sizeof(unsigned long);
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA indexBufferData;
    ZeroMemory(&indexBufferData, sizeof(indexBufferData));
    indexBufferData.pSysMem = &indices.front();

    check(
            device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer_.get()),
            "CreateBuffer failed for index buffer"
            );
}

void Triangle::render(ID3D11DeviceContext* context) const {
    samplerState_.bind(context);
    texture_.bind(context);

    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    context->IASetVertexBuffers(0, 1, &vertexBuffer_.get(), &stride, &offset);
    context->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(vertexCount_, 0, 0);
}

size_t Triangle::vertexCount() const {
    return vertexCount_;
}
