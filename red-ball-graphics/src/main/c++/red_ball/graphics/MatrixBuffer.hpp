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

    D3DXMATRIX& world() {
        return data_.world;
    }

    D3DXMATRIX& view() {
        return data_.view;
    }

    D3DXMATRIX& projection() {
        return data_.projection;
    }

private:

    struct Data {

        D3DXMATRIX world;

        D3DXMATRIX view;

        D3DXMATRIX projection;

    };

    Data data_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_MATRIXBUFFER_HPP_ */
