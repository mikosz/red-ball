#ifndef RED_BALL_GRAPHICS_SHADER_HPP_
#define RED_BALL_GRAPHICS_SHADER_HPP_

#include <stdexcept>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>

#include <D3D11.h>
#include <D3DX10math.h>

#include "red_ball/utils/COMWrapper.hpp"
#include "MatrixBuffer.hpp"
#include "AmbientLightBuffer.hpp"
#include "DirectionalLightBuffer.hpp"
#include "SpecularLightBuffer.hpp"
#include "CameraBuffer.hpp"
#include "Camera.hpp"
#include "GraphicsContext.hpp"
#include "exceptions.hpp"

namespace red_ball {
namespace graphics {

class Shader {
public:

    Shader(
            GraphicsContext* graphicsContext,
            const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
            const boost::filesystem::path& shaderFile,
            const std::string& vertexShaderMain,
            const std::string& pixelShaderMain
            );

    Shader(
            GraphicsContext* graphicsContext,
            const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
            const boost::filesystem::path& vertexShaderFile,
            const std::string& vertexShaderMain,
            const boost::filesystem::path& pixelShaderFile,
            const std::string& pixelShaderMain
            );

    virtual ~Shader() {
    }

    virtual void bind(GraphicsContext* graphicsContext) const = 0;

    virtual void bindWorldMatrix(GraphicsContext* graphicsContext, const D3DMATRIX& worldMatrix) const = 0;

protected:

    void bindShader(GraphicsContext* graphicsContext) const;

private:

    utils::COMWrapper<ID3D11VertexShader> vertexShader_;

    utils::COMWrapper<ID3D11PixelShader> pixelShader_;

    utils::COMWrapper<ID3D11InputLayout> inputLayout_;

    static void Shader::initialise_(
            Shader* shaderPtr,
            GraphicsContext* graphicsContextPtr,
            const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
            const boost::filesystem::path& vertexShaderFile,
            const std::string& vertexShaderMain,
            const boost::filesystem::path& pixelShaderFile,
            const std::string& pixelShaderMain
            );

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
