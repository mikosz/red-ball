#ifndef RED_BALL_GRAPHICS_CONSTANTBUFFER_HPP_
#define RED_BALL_GRAPHICS_CONSTANTBUFFER_HPP_

#include <boost/cstdint.hpp>

#include <D3D11.h>

#include "red_ball/utils/COMWrapper.hpp"

namespace red_ball {
namespace graphics {

class ConstantBuffer {
public:

    virtual ~ConstantBuffer() {
    }

    virtual void bind(ID3D11DeviceContext* deviceContext, size_t bufferNumber) const = 0;

protected:

    ConstantBuffer(ID3D11Device* device, size_t bufferSize);

    void bindData(ID3D11DeviceContext* deviceContext, size_t bufferNumber, const boost::uint8_t* data) const;

private:

    size_t bufferSize_;

    utils::COMWrapper<ID3D11Buffer> buffer_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_CONSTANTBUFFER_HPP_ */
