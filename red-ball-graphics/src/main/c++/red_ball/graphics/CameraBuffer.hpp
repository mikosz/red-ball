#ifndef RED_BALL_GRAPHICS_CAMERABUFFER_HPP_
#define RED_BALL_GRAPHICS_CAMERABUFFER_HPP_

#include <D3D11.h>
#include <D3DX10math.h>

#include "ConstantBuffer.hpp"

namespace red_ball {
namespace graphics {

class CameraBuffer : public ConstantBuffer {
public:

    CameraBuffer(ID3D11Device* device, const D3DXVECTOR3& cameraPosition);

    void bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const;

    D3DXVECTOR3& cameraPosition() {
        return data_.cameraPosition;
    }

private:

    struct Data {

        D3DXVECTOR3 cameraPosition;

        float padding__;

        Data(const D3DXVECTOR3& cameraPosition_);

    };

    Data data_;

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_CAMERABUFFER_HPP_ */
