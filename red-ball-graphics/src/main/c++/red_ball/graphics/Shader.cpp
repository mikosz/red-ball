#include "Shader.hpp"

#include <string>

#include <D3D11.h>
#include <D3DX11async.h>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "red_ball/utils/pointee.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void compileShader(
        utils::COMWrapper<ID3D10Blob>* buffer,
        const boost::filesystem::path& shaderFile,
        const std::string& mainName,
        const std::string& shaderProfile
        ) {
    HRESULT result;
    utils::COMWrapper<ID3D10Blob> errors;

    result = D3DX11CompileFromFileA(
            shaderFile.string().c_str(),
            0,
            0,
            mainName.c_str(),
            shaderProfile.c_str(),
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            0,
            &buffer->get(),
            &errors.get(),
            0
            );

    if (FAILED(result)) {
        if (errors.get()) {
            throw ShaderCompilationError(
                    result,
                    shaderFile,
                    std::string(
                            reinterpret_cast<const char*> (errors->GetBufferPointer())
                            )
            );
        } else if (!boost::filesystem::exists(shaderFile) || !boost::filesystem::is_regular(shaderFile)) {
            throw ShaderCompilationError(result, shaderFile, "File not found, or not a regular file");
        } else {
            throw ShaderCompilationError(result, shaderFile, "Unknown error");
        }
    }
}

void createVertexShader(
        utils::COMWrapper<ID3D11VertexShader>* shader,
        utils::COMWrapper<ID3D11InputLayout>* inputLayout,
        ID3D11Device* device,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
        const boost::filesystem::path& shaderFile,
        const std::string& mainName
        ) {
    HRESULT result;

    utils::COMWrapper<ID3D10Blob> buffer;
    compileShader(&buffer, shaderFile, mainName, "vs_4_0");
    check(device->CreateVertexShader(
            buffer->GetBufferPointer(),
            buffer->GetBufferSize(),
            0,
            &shader->get()
            ),
            "CreateVertexShader failed"
            );

    check(device->CreateInputLayout(
            &inputLayoutDescriptions.front(),
            inputLayoutDescriptions.size(),
            buffer->GetBufferPointer(),
            buffer->GetBufferSize(),
            &inputLayout->get()
            ),
            "CreateInputLayout failed"
            );
}

void createPixelShader(
        utils::COMWrapper<ID3D11PixelShader>* shader,
        ID3D11Device* device,
        const boost::filesystem::path& shaderFile,
        const std::string& mainName
        ) {
    utils::COMWrapper<ID3D10Blob> buffer;
    compileShader(&buffer, shaderFile, mainName, "ps_4_0");
    device->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &shader->get());
}

}  // anonymous namespace

Shader::Shader(
        GraphicsContext* graphicsContextPtr,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
        const boost::filesystem::path& shaderFile,
        const std::string& vertexShaderMain,
        const std::string& pixelShaderMain
        )
{
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    initialise_(
            this,
            &graphicsContext,
            inputLayoutDescriptions,
            shaderFile,
            vertexShaderMain,
            shaderFile,
            pixelShaderMain
            );
}

Shader::Shader(
        GraphicsContext* graphicsContextPtr,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
        const boost::filesystem::path& vertexShaderFile,
        const std::string& vertexShaderMain,
        const boost::filesystem::path& pixelShaderFile,
        const std::string& pixelShaderMain
        ) {
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    initialise_(
            this,
            &graphicsContext,
            inputLayoutDescriptions,
            vertexShaderFile,
            vertexShaderMain,
            pixelShaderFile,
            pixelShaderMain
            );
}

void Shader::bindShader(GraphicsContext* graphicsContextPtr) const {
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);
    ID3D11DeviceContext& deviceContext = graphicsContext.deviceContext();

    deviceContext.IASetInputLayout(inputLayout_);
    deviceContext.VSSetShader(vertexShader_, 0, 0);
    deviceContext.PSSetShader(pixelShader_, 0, 0);
}

void Shader::initialise_(
        Shader* shaderPtr,
        GraphicsContext* graphicsContextPtr,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescriptions,
        const boost::filesystem::path& vertexShaderFile,
        const std::string& vertexShaderMain,
        const boost::filesystem::path& pixelShaderFile,
        const std::string& pixelShaderMain
        )
{
    Shader& shader = utils::pointee(shaderPtr);
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    createVertexShader(
            &shader.vertexShader_,
            &shader.inputLayout_,
            &graphicsContext.device(),
            inputLayoutDescriptions,
            vertexShaderFile,
            vertexShaderMain
            );
    createPixelShader(&shader.pixelShader_, &graphicsContext.device(), pixelShaderFile, pixelShaderMain);
}
