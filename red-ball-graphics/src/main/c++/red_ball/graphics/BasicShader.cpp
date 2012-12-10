#include "BasicShader.hpp"

#include <vector>
#include <stdexcept>

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions() {
    static std::vector<D3D11_INPUT_ELEMENT_DESC> descriptions;

    if (descriptions.empty()) {
        descriptions.resize(2);
        ZeroMemory(&descriptions[0], sizeof(D3D11_INPUT_ELEMENT_DESC));
        ZeroMemory(&descriptions[1], sizeof(D3D11_INPUT_ELEMENT_DESC));

        descriptions[0].SemanticName = "POSITION";
        descriptions[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        descriptions[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        descriptions[0].AlignedByteOffset = 0;

        descriptions[1].SemanticName = "TEXCOORD";
        descriptions[1].Format = DXGI_FORMAT_R32G32_FLOAT;
        descriptions[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        descriptions[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    }

    return descriptions;
}

}  // anonymous namespace

BasicShader::BasicShader(
        ID3D11Device* device,
        const boost::filesystem::path& shaderFile,
        const std::string& vertexShaderMain,
        const std::string& pixelShaderMain
        ) :
        Shader(device, inputElementDescriptions(), shaderFile, vertexShaderMain, pixelShaderMain)
{
}

BasicShader::BasicShader(
        ID3D11Device* device,
        const boost::filesystem::path& vertexShaderFile,
        const std::string& vertexShaderMain,
        const boost::filesystem::path& pixelShaderFile,
        const std::string& pixelShaderMain
        ) :
        Shader(device, inputElementDescriptions(), vertexShaderFile, vertexShaderMain, pixelShaderFile, pixelShaderMain)
{
}
