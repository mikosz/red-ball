#ifndef RED_BALL_GRAPHICS_CAMERA_HPP_
#define RED_BALL_GRAPHICS_CAMERA_HPP_

#include <D3DX10math.h>

namespace red_ball {
namespace graphics {

class Camera {
public:

    Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& rotation);

    void viewMatrix(D3DXMATRIX* matrix) const;

private:

    D3DXVECTOR3 position_;

    D3DXVECTOR3 rotation_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_CAMERA_HPP_ */
