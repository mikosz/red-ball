#ifndef RED_BALL_GRAPHICS_RENDERINGQUEUE_HPP_
#define RED_BALL_GRAPHICS_RENDERINGQUEUE_HPP_

#include <list>

#include <boost/shared_ptr.hpp>

#include <d3d11.h>

#include "Model.hpp"

namespace red_ball {
namespace graphics {

class MatrixBuffer;

class RenderingQueue {
public:

    typedef std::list<boost::shared_ptr<Model> > Models;

    typedef Models::iterator ModelId;

    void render(ID3D11DeviceContext* context, MatrixBuffer* worldMatrixBufferPtr) const;

    ModelId add(boost::shared_ptr<Model> model);

    void remove(ModelId modelId);

private:

    Models models_;

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_RENDERINGQUEUE_HPP_ */
