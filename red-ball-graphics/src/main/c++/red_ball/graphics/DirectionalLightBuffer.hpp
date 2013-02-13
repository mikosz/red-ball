#ifndef RED_BALL_GRAPHICS_DIRECTIONALLIGHTBUFFER_HPP_
#define RED_BALL_GRAPHICS_DIRECTIONALLIGHTBUFFER_HPP_

#include <D3D11.h>
#include <D3DX10math.h>

#include "ConstantBuffer.hpp"

namespace red_ball {
namespace graphics {

class DirectionalLightBuffer : public ConstantBuffer {
public:

    DirectionalLightBuffer(
            ID3D11Device* device,
            const D3DXVECTOR4& diffuseColour,
            const D3DXVECTOR3& lightDirection
            );

    void bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const;

private:

    struct Data {

        D3DXVECTOR4 diffuseColour;

        D3DXVECTOR3 lightDirection;

        float padding__; // extra padding required by CreateBuffer

        Data(const D3DXVECTOR4& diffuseColour_, const D3DXVECTOR3& lightDirection_);

    };

    Data data_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_DIRECTIONALLIGHTBUFFER_HPP_ */
