#ifndef RED_BALL_GRAPHICS_MODEL_HPP_
#define RED_BALL_GRAPHICS_MODEL_HPP_

#include <d3d11.h>
#include <D3DX10math.h>

namespace red_ball {
namespace graphics {

class MatrixBuffer;

class Model {
public:

    virtual ~Model() {
    }

    virtual void render(ID3D11DeviceContext* context, MatrixBuffer* worldMatrixBuffer) const = 0;

    virtual size_t vertexCount() const = 0;

    void moveTo(const D3DXVECTOR3& location) {
        translation_ = location;
    }

    void rotateBy(const D3DXVECTOR3& radians);

    void scaleBy(const D3DXVECTOR3& times) {
        scale_ += times;
    }

protected:

    Model();

    void adjust(MatrixBuffer* worldMatrixBuffer) const;

private:

    D3DXVECTOR3 translation_;

    D3DXVECTOR3 rotation_;

    D3DXVECTOR3 scale_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_MODEL_HPP_ */
