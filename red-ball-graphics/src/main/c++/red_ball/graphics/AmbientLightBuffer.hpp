#ifndef RED_BALL_GRAPHICS_AMBIENTLIGHTBUFFER_HPP_
#define RED_BALL_GRAPHICS_AMBIENTLIGHTBUFFER_HPP_

#include <D3D11.h>
#include <D3DX10math.h>

#include "ConstantBuffer.hpp"

namespace red_ball {
namespace graphics {

class AmbientLightBuffer : public ConstantBuffer {
public:

    AmbientLightBuffer(
            ID3D11Device* device,
            const D3DXVECTOR4& ambientColour
            );

    void bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const;

private:

    D3DXVECTOR4 ambientColour_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_AMBIENTLIGHTBUFFER_HPP_ */
