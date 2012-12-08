#ifndef RED_BALL_GRAPHICS_SHADER_HPP_
#define RED_BALL_GRAPHICS_SHADER_HPP_

#include <stdexcept>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <D3D11.h>
#include <D3DX10math.h>

#include "red_ball/utils/COMWrapper.hpp"
#include "exceptions.hpp"

namespace red_ball {
namespace graphics {

class Shader {
public:

    Shader(
            ID3D11Device* device,
            const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
            const boost::filesystem::path& shaderFile,
            const std::string& vertexShaderMain,
            const std::string& pixelShaderMain
            );

    Shader(
            ID3D11Device* device,
            const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
            const boost::filesystem::path& vertexShaderFile,
            const std::string& vertexShaderMain,
            const boost::filesystem::path& pixelShaderFile,
            const std::string& pixelShaderMain
            );

    virtual ~Shader() {
    }

    void bind(ID3D11DeviceContext* deviceContext) const;

private:

    utils::COMWrapper<ID3D11VertexShader> vertexShader_;

    utils::COMWrapper<ID3D11PixelShader> pixelShader_;

    utils::COMWrapper<ID3D11InputLayout> inputLayout_;

};

class ShaderCompilationError : public DirectXException {
public:

    ShaderCompilationError(HRESULT hResult, const boost::filesystem::path& path, const std::string& error) :
        DirectXException(hResult, "Failed to compile shader file " + path.string() + ", error: " + error) {
    }

    ~ShaderCompilationError() throw () {
    }

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_SHADER_HPP_ */
