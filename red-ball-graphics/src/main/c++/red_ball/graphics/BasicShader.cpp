#include "BasicShader.hpp"

#include <vector>
#include <stdexcept>

#include "red_ball/utils/pointee.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions() {
    static std::vector<D3D11_INPUT_ELEMENT_DESC> descriptions;

    if (descriptions.empty()) {
        descriptions.resize(3);
        ZeroMemory(&descriptions[0], sizeof(D3D11_INPUT_ELEMENT_DESC));
        ZeroMemory(&descriptions[1], sizeof(D3D11_INPUT_ELEMENT_DESC));
        ZeroMemory(&descriptions[2], sizeof(D3D11_INPUT_ELEMENT_DESC));

        descriptions[0].SemanticName = "POSITION";
        descriptions[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        descriptions[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        descriptions[0].AlignedByteOffset = 0;

        descriptions[1].SemanticName = "TEXCOORD";
        descriptions[1].Format = DXGI_FORMAT_R32G32_FLOAT;
        descriptions[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        descriptions[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

        descriptions[2].SemanticName = "NORMAL";
        descriptions[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        descriptions[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        descriptions[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    }

    return descriptions;
}

void setupPerspectiveProjectionMatrix(
        D3DXMATRIX* matrix,
        size_t screenWidth,
        size_t screenHeight
        ) {
    float aspectRatio = static_cast<float> (screenWidth) / screenHeight;
    D3DXMatrixPerspectiveFovLH(matrix, D3DX_PI / 4.0f, aspectRatio, 1.0f, 100.0f);
}

void setupOrthographicProjectionMatrix(
        D3DXMATRIX* matrix,
        size_t screenWidth,
        size_t screenHeight
        ) {
    D3DXMatrixOrthoLH(matrix, screenWidth, screenHeight, 1.0f, 100.0f);
}

}  // anonymous namespace

BasicShader::BasicShader(
        GraphicsContext* graphicsContextPtr,
        const boost::filesystem::path& shaderFile,
        const std::string& vertexShaderMain,
        const std::string& pixelShaderMain
        ) :
        Shader(graphicsContextPtr, inputElementDescriptions(), shaderFile, vertexShaderMain, pixelShaderMain),
        camera_(
               D3DXVECTOR3(0.0f, 0.0f, -10.0f),
               D3DXVECTOR3(0.0f, 0.0f, 0.0f)
               )
{
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    initialise_(
            this,
            &graphicsContext
            );
}

BasicShader::BasicShader(
        GraphicsContext* graphicsContextPtr,
        const boost::filesystem::path& vertexShaderFile,
        const std::string& vertexShaderMain,
        const boost::filesystem::path& pixelShaderFile,
        const std::string& pixelShaderMain
        ) :
        Shader(
                graphicsContextPtr,
                inputElementDescriptions(),
                vertexShaderFile,
                vertexShaderMain,
                pixelShaderFile,
                pixelShaderMain
                ),
        camera_(
               D3DXVECTOR3(0.0f, 0.0f, -10.0f),
               D3DXVECTOR3(0.0f, 0.0f, 0.0f)
               )
{
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    initialise_(
            this,
            &graphicsContext
            );
}

void BasicShader::bind(GraphicsContext* graphicsContextPtr) const {
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);
    ID3D11DeviceContext& deviceContext = graphicsContext.deviceContext();

    camera_.viewMatrix(&viewMatrixBuffer_->matrix());
    viewMatrixBuffer_->bind(&graphicsContext.deviceContext(), 1);
    perspectiveProjectionMatrixBuffer_->bind(&graphicsContext.deviceContext(), 2);
    cameraBuffer_->cameraPosition() = camera_.position();
    cameraBuffer_->bind(&graphicsContext.deviceContext(), 3);

    ambientLightBuffer_->bind(&graphicsContext.deviceContext(), 0);
    directionalLightBuffer_->bind(&graphicsContext.deviceContext(), 1);
    specularLightBuffer_->bind(&graphicsContext.deviceContext(), 2);

    bindShader(&graphicsContext);
}

void BasicShader::bindWorldMatrix(GraphicsContext* graphicsContextPtr, const D3DMATRIX& worldMatrix) const {
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    worldMatrixBuffer_->matrix() = worldMatrix;
    worldMatrixBuffer_->bind(&graphicsContext.deviceContext(), 0);
}

void BasicShader::initialise_(
        BasicShader* shaderPtr,
        GraphicsContext* graphicsContextPtr
        ) {
    BasicShader& shader = utils::pointee(shaderPtr);
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    shader.orthographicProjectionMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));
    setupOrthographicProjectionMatrix(
            &shader.orthographicProjectionMatrixBuffer_->matrix(),
            graphicsContext.displayInfo().resolution().first,
            graphicsContext.displayInfo().resolution().second
            );

    shader.perspectiveProjectionMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));
    setupPerspectiveProjectionMatrix(
            &shader.perspectiveProjectionMatrixBuffer_->matrix(),
            graphicsContext.displayInfo().resolution().first,
            graphicsContext.displayInfo().resolution().second
            );

    shader.viewMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));

    shader.worldMatrixBuffer_.reset(new MatrixBuffer(&graphicsContext.device()));

    shader.ambientLightBuffer_.reset(
            new AmbientLightBuffer(&graphicsContext.device(), D3DXVECTOR4(0.15f, 0.15f, 0.15f, 1.0f)));

    shader.directionalLightBuffer_.reset(
            new DirectionalLightBuffer(&graphicsContext.device(), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f),
                    D3DXVECTOR3(-1.0f, -1.0f, 1.0f)));

    shader.specularLightBuffer_.reset(
            new SpecularLightBuffer(&graphicsContext.device(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 16.0f));

    shader.cameraBuffer_.reset(new CameraBuffer(&graphicsContext.device(), shader.camera_.position()));
}
