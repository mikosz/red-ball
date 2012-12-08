#ifndef RED_BALL_GRAPHICS_TRIANGLE_HPP_
#define RED_BALL_GRAPHICS_TRIANGLE_HPP_

#include "red_ball/utils/COMWrapper.hpp"
#include "Model.hpp"

namespace red_ball {
namespace graphics {

class Triangle : public Model {
public:

    Triangle(ID3D11Device* device);

    void render(ID3D11DeviceContext* context) const;

    size_t vertexCount() const;

private:

    utils::COMWrapper<ID3D11Buffer> vertexBuffer_;

    utils::COMWrapper<ID3D11Buffer> indexBuffer_;

    size_t vertexCount_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_TRIANGLE_HPP_ */
