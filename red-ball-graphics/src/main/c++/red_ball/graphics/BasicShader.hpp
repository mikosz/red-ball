#ifndef RED_BALL_GRAPHICS_BASICSHADER_HPP_
#define RED_BALL_GRAPHICS_BASICSHADER_HPP_

#include "Shader.hpp"

namespace red_ball {
namespace graphics {

class BasicShader : public Shader {
public:

    BasicShader(
            GraphicsContext* graphicsContext,
            const boost::filesystem::path& shaderFile,
            const std::string& vertexShaderMain,
            const std::string& pixelShaderMain
            );

    BasicShader(
            GraphicsContext* graphicsContext,
            const boost::filesystem::path& vertexShaderFile,
            const std::string& vertexShaderMain,
            const boost::filesystem::path& pixelShaderFile,
            const std::string& pixelShaderMain
            );

    void bind(GraphicsContext* graphicsContext) const;

    void bindWorldMatrix(GraphicsContext* graphicsContext, const D3DMATRIX& worldMatrix) const;

private:

    boost::scoped_ptr<MatrixBuffer> worldMatrixBuffer_;

    boost::scoped_ptr<MatrixBuffer> perspectiveProjectionMatrixBuffer_;

    boost::scoped_ptr<MatrixBuffer> orthographicProjectionMatrixBuffer_;

    boost::scoped_ptr<MatrixBuffer> viewMatrixBuffer_;

    boost::scoped_ptr<AmbientLightBuffer> ambientLightBuffer_;

    boost::scoped_ptr<DirectionalLightBuffer> directionalLightBuffer_;

    boost::scoped_ptr<SpecularLightBuffer> specularLightBuffer_;

    boost::scoped_ptr<CameraBuffer> cameraBuffer_;

    Camera camera_;

    static void initialise_(
            BasicShader* shader,
            GraphicsContext* graphicsContext
            );

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_BASICSHADER_HPP_ */
