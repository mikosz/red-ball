#ifndef RED_BALL_GRAPHICS_VERTEX_HPP_
#define RED_BALL_GRAPHICS_VERTEX_HPP_

#include <D3DX10math.h>

namespace red_ball {
namespace graphics {

struct Vertex {

    D3DXVECTOR3 position;

    D3DXVECTOR2 textureCoordinates;

    D3DXVECTOR3 normal;

    Vertex() {
    }

    Vertex(
            const D3DXVECTOR3& position_,
            const D3DXVECTOR2& textureCoordinates_,
            const D3DXVECTOR3& normal_
            ) :
        position(position_),
        textureCoordinates(textureCoordinates_),
        normal(normal_)
    {
    }

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_VERTEX_HPP_ */
