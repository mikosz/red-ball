#ifndef RED_BALL_GRAPHICS_SPECULARLIGHTBUFFER_HPP_
#define RED_BALL_GRAPHICS_SPECULARLIGHTBUFFER_HPP_

#include <D3D11.h>
#include <D3DX10math.h>

#include "ConstantBuffer.hpp"

namespace red_ball {
namespace graphics {

class SpecularLightBuffer : public ConstantBuffer {
public:

    SpecularLightBuffer(
            ID3D11Device* device,
            const D3DXVECTOR4& specularColour,
            float specularPower
            );

    void bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const;

private:

    struct Data {

        D3DXVECTOR4 specularColour;

        float specularPower;

        D3DXVECTOR3 padding__;

        Data(const D3DXVECTOR4& specularColour_, float specularPower_);

    };

    Data data_;

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_SPECULARLIGHTBUFFER_HPP_ */
