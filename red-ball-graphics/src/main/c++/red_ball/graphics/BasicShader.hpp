#ifndef RED_BALL_GRAPHICS_BASICSHADER_HPP_
#define RED_BALL_GRAPHICS_BASICSHADER_HPP_

#include "Shader.hpp"

namespace red_ball {
namespace graphics {

class BasicShader : public Shader {
public:

    BasicShader(
            ID3D11Device* device,
            const boost::filesystem::path& shaderFile,
            const std::string& vertexShaderMain,
            const std::string& pixelShaderMain
            );

    BasicShader(
            ID3D11Device* device,
            const boost::filesystem::path& vertexShaderFile,
            const std::string& vertexShaderMain,
            const boost::filesystem::path& pixelShaderFile,
            const std::string& pixelShaderMain
            );

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_BASICSHADER_HPP_ */
