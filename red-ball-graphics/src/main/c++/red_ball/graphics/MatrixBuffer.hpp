#ifndef RED_BALL_GRAPHICS_MATRIXBUFFER_HPP_
#define RED_BALL_GRAPHICS_MATRIXBUFFER_HPP_

#include <D3DX11.h>
#include <D3DX10math.h>

#include "ConstantBuffer.hpp"

namespace red_ball {
namespace graphics {

class MatrixBuffer : public ConstantBuffer {
public:

    MatrixBuffer(ID3D11Device* device);

    void bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const;

    D3DXMATRIX& matrix() {
        return matrix_;
    }

private:

    D3DXMATRIX matrix_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_MATRIXBUFFER_HPP_ */
