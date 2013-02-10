#ifndef RED_BALL_GRAPHICS_BASICMODEL_HPP_
#define RED_BALL_GRAPHICS_BASICMODEL_HPP_

#include "red_ball/utils/COMWrapper.hpp"
#include "Model.hpp"
#include "ModelLoader.hpp"
#include "Texture.hpp"
#include "SamplerState.hpp"

namespace red_ball {
namespace graphics {

class BasicModel : public Model {
public:

    BasicModel(ID3D11Device* device, const ModelLoader& modelLoader);

    void render(ID3D11DeviceContext* context) const;

    size_t vertexCount() const;

private:

    Texture texture_;

    SamplerState samplerState_;

    size_t vertexCount_;

    utils::COMWrapper<ID3D11Buffer> vertexBuffer_;

    utils::COMWrapper<ID3D11Buffer> indexBuffer_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_BASICMODEL_HPP_ */
