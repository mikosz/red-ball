#include "Texture.hpp"

#include "exceptions.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void setupResourceView(
        utils::COMWrapper<ID3D11ShaderResourceView>* texture,
        ID3D11Device* device,
        const boost::filesystem::path& path
        ) {
    check(
            D3DX11CreateShaderResourceViewFromFileA(device, path.string().c_str(), 0, 0, &texture->get(), 0),
            "CreateShaderResourceView failed"
            );
}

} // anonymous namespace

Texture::Texture(ID3D11Device* device, const boost::filesystem::path& path) {
    setupResourceView(&texture_, device, path);
}

void Texture::bind(ID3D11DeviceContext* deviceContext) const {
    deviceContext->PSSetShaderResources(0, 1, &texture_.get());
}
