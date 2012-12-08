#ifndef RED_BALL_GRAPHICS_MODEL_HPP_
#define RED_BALL_GRAPHICS_MODEL_HPP_

#include <d3d11.h>

namespace red_ball {
namespace graphics {

class Model {
public:

    virtual void render(ID3D11DeviceContext* context) const = 0;

    virtual size_t vertexCount() const = 0;

protected:

    Model() {
    }

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_MODEL_HPP_ */
