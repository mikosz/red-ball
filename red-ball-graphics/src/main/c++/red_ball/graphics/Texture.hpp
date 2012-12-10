#ifndef RED_BALL_GRAPHICS_TEXTURE_HPP_
#define RED_BALL_GRAPHICS_TEXTURE_HPP_

#include <boost/filesystem/path.hpp>

#include <d3d11.h>
#include <D3DX11tex.h>

#include "red_ball/utils/COMWrapper.hpp"

namespace red_ball {
namespace graphics {

class Texture {
public:

    Texture(ID3D11Device* device, const boost::filesystem::path& path);

    void bind(ID3D11DeviceContext* deviceContext) const;

private:

    utils::COMWrapper<ID3D11ShaderResourceView> texture_;

};

}  // namespace graphics
}  // namespace red_ball

#endif /* RED_BALL_GRAPHICS_TEXTURE_HPP_ */
