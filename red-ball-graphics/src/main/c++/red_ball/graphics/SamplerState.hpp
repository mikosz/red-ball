#ifndef RED_BALL_GRAPHICS_SAMPLERSTATE_HPP_
#define RED_BALL_GRAPHICS_SAMPLERSTATE_HPP_

#include <d3d11.h>

#include "red_ball/utils/COMWrapper.hpp"

namespace red_ball {
namespace graphics {

class SamplerState {
public:

    SamplerState(ID3D11Device* device);

    void bind(ID3D11DeviceContext* deviceContext) const;

private:

    utils::COMWrapper<ID3D11SamplerState> samplerState_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_SAMPLERSTATE_HPP_ */
